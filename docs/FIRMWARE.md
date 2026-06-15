# Firmware Development Guide

Detailed guide for developing and extending the ESP32-S3 firmware.

## Architecture

### Main Components

1. **Audio Module** - Handles I2S microphone and speaker
2. **Wake Word Detection** - ESP-SR offline detection
3. **Communication** - WebSocket and MQTT protocols
4. **Display** - OLED/LCD interface
5. **Power Management** - Battery monitoring and control
6. **MCP** - Device control interface

## File Structure

```
firmware/
├── ESP32_S3_Voice_Assistant.ino    # Main sketch
├── config/
│   ├── config.h                    # System configuration
│   └── pins.h                      # GPIO pinout
└── src/
    ├── audio/
    │   ├── i2s_handler.cpp
    │   ├── opus_encoder.cpp
    │   └── audio_processor.cpp
    ├── ml/
    │   ├── wake_word_detector.cpp
    │   └── speaker_recognition.cpp
    ├── comms/
    │   ├── websocket_client.cpp
    │   ├── mqtt_client.cpp
    │   └── protocol_handler.cpp
    ├── display/
    │   ├── oled_driver.cpp
    │   ├── ui_manager.cpp
    │   └── emoji_renderer.cpp
    ├── power/
    │   ├── battery_monitor.cpp
    │   └── power_manager.cpp
    ├── mcp/
    │   ├── device_mcp.cpp
    │   └── mcp_protocol.cpp
    └── utils/
        ├── logger.cpp
        └── helpers.cpp
```

## Development Workflow

### 1. Setup Development Environment

```bash
# Clone repository
git clone https://github.com/Probalwahid/ESP32S3-AI-voice-assistant.git
cd ESP32S3-AI-voice-assistant/firmware
```

### 2. Install Dependencies

In Arduino IDE:
- Install ESP32 Board Package v2.0.0+
- Install required libraries:
  - I2S
  - ArduinoJson
  - WebSocket
  - PubSubClient

### 3. Configure for Your Board

Edit `config/config.h`:
```cpp
#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"
#define BACKEND_HOST "192.168.1.100"
```

Edit `config/pins.h` if using different GPIO:
```cpp
#define I2S_MIC_BCK 1
#define I2S_MIC_WS 2
#define I2S_MIC_DATA 42
```

### 4. Build and Upload

1. Open `ESP32_S3_Voice_Assistant.ino` in Arduino IDE
2. Select Board: **ESP32S3 Dev Module**
3. Select COM port
4. Click **Upload**

## Key Components

### Audio Processing Pipeline

```
INMP441 Microphone
    ↓ (I2S0, 16kHz, 32-bit)
Audio Buffer
    ↓
Wake Word Detection (ESP-SR)
    ↓ (if detected)
OPUS Encoding
    ↓
WebSocket/MQTT Send to Backend
    ↓
Backend Processing (ASR, LLM, TTS)
    ↓
OPUS Decoding
    ↓ (I2S1, 16kHz, 16-bit)
MAX98357A Speaker
```

### Wake Word Detection

Using ESP-SR library:
```cpp
#include "esp_sr.h"

void detectWakeWord() {
  // Initialize ESP-SR model
  esp_sr_iface_t *iface = &ESP_SR_MULTINET;
  
  // Detect wake word
  if (iface->detect(audio_buffer)) {
    // Wake word detected
    isListening = true;
  }
}
```

### Communication

#### WebSocket
```cpp
WebSocketsClient webSocket;

void connectWebSocket() {
  webSocket.begin(BACKEND_HOST, BACKEND_PORT, "/audio");
  webSocket.onEvent(webSocketEvent);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    // Process incoming command
  }
}
```

#### MQTT
```cpp
PubSubClient mqttClient(wifiClient);

void connectMQTT() {
  if (!mqttClient.connected()) {
    mqttClient.connect(MQTT_CLIENT_ID);
    mqttClient.subscribe("device/commands/#");
  }
}

void publishAudio(uint8_t *data, size_t length) {
  mqttClient.publish("device/audio/input", data, length);
}
```

### Display Management

```cpp
#include <Adafruit_SSD1306.h>

void initDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void updateDisplay(const char *message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}
```

## Testing

### Serial Monitor
```bash
Monitor at 115200 baud to see debug output
```

### Troubleshooting

1. **No Audio Input**
   - Check I2S pins
   - Verify microphone power (3.3V)
   - Monitor audio levels

2. **No Audio Output**
   - Check speaker connections
   - Verify speaker power (5V)
   - Test with known working speaker

3. **Wake Word Not Detecting**
   - Check audio input quality
   - Adjust sensitivity in config.h
   - Verify ESP-SR model is loaded

## Performance Optimization

1. **Memory Management**
   - Use PSRAM for large buffers
   - Monitor heap usage
   - Free unused memory

2. **Power Consumption**
   - Optimize CPU frequency
   - Disable unused peripherals
   - Implement sleep modes

3. **Audio Quality**
   - Use appropriate sample rate (16kHz for most applications)
   - Optimize OPUS bitrate
   - Reduce latency

## Advanced Features

### Custom Wake Words

Update wake word in `config/config.h`:
```cpp
#define WAKE_WORD "custom_word"
```

Train custom model using ESP-SR tools.

### Multi-Language Support

```cpp
#define LANGUAGE "bangla"  // "english", "bangla", "hindi"

// Load language-specific models
void initializeLanguage() {
  // Load appropriate language model
}
```

### Speaker Recognition

```cpp
void recognizeSpeaker(uint8_t *audioData, size_t length) {
  // Process with speaker recognition model
  // Return speaker ID and confidence
}
```

## Contributing

1. Create feature branch
2. Make changes
3. Test thoroughly
4. Submit pull request

## References

- [ESP32 I2S Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html)
- [ESP-SR GitHub](https://github.com/espressif/esp-sr)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
