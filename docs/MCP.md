# Model Context Protocol (MCP) Guide

Implementation guide for Model Context Protocol (MCP) in the voice assistant.

## Overview

MCP (Model Context Protocol) enables the AI model to:
1. **Control devices** - LED, servo, GPIO on the ESP32
2. **Execute cloud actions** - Smart home control, web searches, etc.
3. **Access external services** - Email, calendar, knowledge bases
4. **Extend capabilities** - Integrate with third-party APIs

## Architecture

```
LLM (on Backend)
   ↓
MCP Handler (Backend)
   ├─ Device MCP (On-Device Control)
   │  └─ ESP32 MCP Executor
   │     ├─ LED Control
   │     ├─ Servo Control
   │     ├─ GPIO Control
   │     └─ Speaker Control
   │
   └─ Cloud MCP (Cloud Services)
      ├─ Smart Home Control
      ├─ Web Search
      ├─ Email Service
      ├─ Calendar Service
      └─ Knowledge Base
```

## Device MCP

Controls hardware components on ESP32.

### Available Device Commands

#### LED Control

```json
{
  "type": "device_mcp",
  "command": "led_control",
  "params": {
    "action": "on|off|blink",
    "color": "red|green|blue",
    "brightness": 0-255,
    "duration": 1000
  }
}
```

#### Servo Control

```json
{
  "type": "device_mcp",
  "command": "servo_control",
  "params": {
    "angle": 0-180,
    "speed": 1-10,
    "duration": 1000
  }
}
```

#### GPIO Control

```json
{
  "type": "device_mcp",
  "command": "gpio_control",
  "params": {
    "pin": 1-40,
    "mode": "OUTPUT|INPUT",
    "value": "HIGH|LOW",
    "duration": 1000
  }
}
```

#### Speaker Control

```json
{
  "type": "device_mcp",
  "command": "speaker_control",
  "params": {
    "volume": 0-100,
    "mute": true|false
  }
}
```

### Device MCP Implementation

#### Firmware (Arduino)

```cpp
// firmware/src/mcp/device_mcp.cpp

void handleDeviceMCP(const JsonDocument& mcp) {
  String command = mcp["command"];
  
  if (command == "led_control") {
    handleLEDControl(mcp["params"]);
  }
  else if (command == "servo_control") {
    handleServoControl(mcp["params"]);
  }
  else if (command == "gpio_control") {
    handleGPIOControl(mcp["params"]);
  }
}

void handleLEDControl(const JsonObject& params) {
  String action = params["action"];
  
  if (action == "on") {
    digitalWrite(LED_PIN, HIGH);
  }
  else if (action == "off") {
    digitalWrite(LED_PIN, LOW);
  }
  else if (action == "blink") {
    // Blink LED with specified duration
  }
}
```

#### Backend (Python)

```python
# backend/services/device_mcp.py

class DeviceMCPHandler:
    def __init__(self, mqtt_client):
        self.mqtt = mqtt_client
    
    def execute_command(self, command, params):
        """Execute device MCP command"""
        payload = {
            "command": command,
            "params": params
        }
        
        # Send to device via MQTT
        self.mqtt.publish(
            f"device/mcp/{command}",
            json.dumps(payload)
        )
        
        return {"status": "sent", "command": command}
```

## Cloud MCP

Accesses external services and cloud APIs.

### Available Cloud Commands

#### Smart Home Control

```json
{
  "type": "cloud_mcp",
  "command": "smart_home",
  "action": "get_devices|control_device|set_scene",
  "params": {
    "device_id": "light_1",
    "command": "on|off|dim",
    "level": 0-100
  }
}
```

#### Web Search

```json
{
  "type": "cloud_mcp",
  "command": "web_search",
  "params": {
    "query": "search query",
    "limit": 5
  }
}
```

#### Email Service

```json
{
  "type": "cloud_mcp",
  "command": "email",
  "action": "send|read|check",
  "params": {
    "to": "recipient@example.com",
    "subject": "Email subject",
    "body": "Email body"
  }
}
```

#### Weather Service

```json
{
  "type": "cloud_mcp",
  "command": "weather",
  "params": {
    "location": "New York",
    "unit": "celsius|fahrenheit"
  }
}
```

### Cloud MCP Implementation

```python
# backend/services/cloud_mcp.py

class CloudMCPHandler:
    def __init__(self):
        self.search_client = GoogleSearch()
        self.email_service = GmailAPI()
        self.weather_api = WeatherAPI()
    
    def execute_command(self, command, params):
        """Execute cloud MCP command"""
        
        if command == "web_search":
            return self.web_search(params)
        elif command == "email":
            return self.handle_email(params)
        elif command == "weather":
            return self.get_weather(params)
        else:
            return {"error": "Unknown command"}
    
    def web_search(self, params):
        """Search the web"""
        query = params.get("query")
        limit = params.get("limit", 5)
        
        results = self.search_client.search(query, limit)
        return {
            "status": "success",
            "results": results
        }
    
    def handle_email(self, params):
        """Handle email operations"""
        action = params.get("action")
        
        if action == "send":
            return self.email_service.send(
                to=params.get("to"),
                subject=params.get("subject"),
                body=params.get("body")
            )
        elif action == "read":
            return self.email_service.get_recent()
        else:
            return {"error": "Unknown action"}
    
    def get_weather(self, params):
        """Get weather information"""
        location = params.get("location")
        unit = params.get("unit", "celsius")
        
        weather = self.weather_api.get_weather(location, unit)
        return {
            "status": "success",
            "weather": weather
        }
```

## LLM Integration with MCP

The LLM uses MCP to accomplish tasks:

```python
# backend/routes/llm.py

from services.device_mcp import DeviceMCPHandler
from services.cloud_mcp import CloudMCPHandler

device_mcp = DeviceMCPHandler(mqtt_client)
cloud_mcp = CloudMCPHandler()

def chat_with_mcp(user_message):
    """Chat with MCP support"""
    
    # Build prompt with MCP tools available
    system_prompt = """
    You are an AI assistant. You have access to these tools:
    
    Device Controls:
    - led_control: Turn LED on/off, set brightness
    - servo_control: Move servo to specific angle
    - gpio_control: Control GPIO pins
    - speaker_control: Adjust volume
    
    Cloud Services:
    - web_search: Search the internet
    - email: Send and receive emails
    - weather: Get weather information
    - smart_home: Control smart home devices
    
    When you need to use a tool, respond with:
    <mcp_call>
    {"type": "device_mcp", "command": "...", "params": {...}}
    </mcp_call>
    """
    
    # Get LLM response
    response = llm.chat(
        system_prompt=system_prompt,
        message=user_message
    )
    
    # Execute any MCP calls in response
    mcp_calls = extract_mcp_calls(response)
    for call in mcp_calls:
        if call["type"] == "device_mcp":
            device_mcp.execute_command(call["command"], call["params"])
        elif call["type"] == "cloud_mcp":
            cloud_mcp.execute_command(call["command"], call["params"])
    
    return response
```

## Example Interactions

### Turn on the Light

```
User: "Turn on the living room light"

LLM (with MCP):
"I'll turn on the living room light for you."
<mcp_call>
{"type": "cloud_mcp", "command": "smart_home", "action": "control_device", "params": {"device_id": "light_living_room", "command": "on"}}
</mcp_call>
```

### Search the Web

```
User: "What's the weather like today?"

LLM (with MCP):
"Let me check the weather for you."
<mcp_call>
{"type": "cloud_mcp", "command": "weather", "params": {"location": "current_location"}}
</mcp_call>
```

## Security Considerations

1. **Input Validation**
   - Validate all MCP parameters
   - Whitelist allowed commands
   - Limit parameter ranges

2. **Access Control**
   - Restrict MCP commands by user
   - Require authentication
   - Log all MCP executions

3. **Rate Limiting**
   - Limit MCP calls per minute
   - Prevent command spam
   - Monitor resource usage

## Testing MCP

```python
# tests/test_mcp.py

import pytest
from services.device_mcp import DeviceMCPHandler

def test_led_control():
    mcp = DeviceMCPHandler(mock_mqtt)
    result = mcp.execute_command(
        "led_control",
        {"action": "on", "brightness": 255}
    )
    assert result["status"] == "sent"

def test_web_search():
    mcp = CloudMCPHandler()
    result = mcp.web_search({"query": "python programming"})
    assert result["status"] == "success"
    assert len(result["results"]) > 0
```

## References

- [Model Context Protocol](https://modelcontextprotocol.io/)
- [OpenAI Function Calling](https://platform.openai.com/docs/guides/function-calling)
