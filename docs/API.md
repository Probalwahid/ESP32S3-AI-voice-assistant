# API Documentation

Complete REST API and WebSocket API documentation.

## Base URL

```
HTTP: http://localhost:5000
WebSocket: ws://localhost:8080
```

## Authentication

Current implementation does not require authentication. Future versions will support API keys.

## Endpoints

### Health Check

```
GET /health
```

**Response:**
```json
{
  "status": "healthy",
  "service": "ESP32-S3 AI Voice Assistant Backend",
  "version": "1.0.0"
}
```

### Audio Transcription (ASR)

```
POST /api/audio/transcribe
Content-Type: audio/wav or audio/opus

Body: Binary audio data
```

**Response:**
```json
{
  "status": "success",
  "text": "hello world",
  "confidence": 0.95,
  "language": "en"
}
```

**Errors:**
- 400: Invalid audio format
- 413: Audio file too large
- 500: Processing error

### Text-to-Speech (TTS)

```
POST /api/tts/synthesize
Content-Type: application/json
```

**Request Body:**
```json
{
  "text": "Hello, how can I help you?",
  "language": "en",
  "voice": "default",
  "speed": 1.0
}
```

**Response:**
```json
{
  "status": "success",
  "audio": "base64_encoded_opus_data",
  "format": "opus",
  "duration": 2.5
}
```

### LLM Chat

```
POST /api/llm/chat
Content-Type: application/json
```

**Request Body:**
```json
{
  "message": "What is the weather?",
  "context": [
    {"role": "assistant", "content": "Hello! How can I help?"},
    {"role": "user", "content": "Hi there"}
  ],
  "language": "en",
  "max_tokens": 150
}
```

**Response:**
```json
{
  "status": "success",
  "response": "I don't have access to real-time weather data...",
  "tokens_used": 45,
  "model": "gpt-3.5-turbo"
}
```

### Speaker Identification

```
POST /api/speaker/identify
Content-Type: audio/wav or audio/opus

Body: Binary audio data
```

**Response:**
```json
{
  "status": "success",
  "speaker_id": "speaker_001",
  "confidence": 0.92,
  "name": "John Doe"
}
```

### Device Control

```
POST /api/device/control
Content-Type: application/json
```

**Request Body:**
```json
{
  "device": "led",
  "action": "on",
  "parameters": {
    "color": "red",
    "brightness": 255
  }
}
```

**Response:**
```json
{
  "status": "success",
  "message": "Device command sent"
}
```

### MCP Execute

```
POST /api/mcp/execute
Content-Type: application/json
```

**Request Body:**
```json
{
  "command": "get_home_devices",
  "params": {}
}
```

**Response:**
```json
{
  "status": "success",
  "result": {
    "devices": [
      {"id": "light_1", "name": "Living Room Light", "state": "on"}
    ]
  }
}
```

## WebSocket API

### Connection

```javascript
const ws = new WebSocket('ws://localhost:8080/audio');

ws.onopen = () => {
  console.log('Connected');
};

ws.onmessage = (event) => {
  console.log('Message received:', event.data);
};

ws.onerror = (error) => {
  console.error('Error:', error);
};
```

### Message Format

**Send Audio:**
```json
{
  "type": "audio",
  "format": "opus",
  "data": "base64_encoded_audio_data"
}
```

**Receive Response:**
```json
{
  "type": "transcription",
  "text": "hello",
  "confidence": 0.95
}
```

**Receive Audio Output:**
```json
{
  "type": "audio",
  "format": "opus",
  "data": "base64_encoded_audio_data"
}
```

## MQTT Topics

### Device to Server

- `device/audio/input` - Audio stream input
- `device/status` - Device status
- `device/metrics` - Performance metrics

### Server to Device

- `device/audio/output` - Audio stream output
- `device/control/led` - LED control commands
- `device/control/servo` - Servo control commands
- `device/control/gpio` - GPIO control commands

## Error Handling

### Error Response Format

```json
{
  "status": "error",
  "error_code": "ASR_FAILED",
  "message": "Failed to process audio",
  "details": "Invalid audio format"
}
```

### Error Codes

| Code | HTTP | Description |
|------|------|-------------|
| INVALID_REQUEST | 400 | Invalid request format |
| INVALID_AUDIO | 400 | Invalid audio data |
| AUTH_FAILED | 401 | Authentication failed |
| RESOURCE_NOT_FOUND | 404 | Resource not found |
| RATE_LIMIT | 429 | Rate limit exceeded |
| ASR_FAILED | 500 | ASR processing failed |
| TTS_FAILED | 500 | TTS processing failed |
| LLM_FAILED | 500 | LLM processing failed |
| SERVER_ERROR | 500 | Internal server error |

## Rate Limiting

Current implementation has no rate limiting. Production deployment should implement:

- Per-IP rate limiting
- Per-API-key rate limiting
- Concurrent request limits

## CORS

Currently allows all origins. Configure for production:

```python
from flask_cors import CORS
CORS(app, origins=['https://yourdomain.com'])
```

## Versioning

API endpoints may be versioned in future:

```
/api/v1/audio/transcribe
/api/v2/audio/transcribe
```

## Changelog

### v1.0.0
- Initial API release
- Basic ASR, TTS, LLM endpoints
- WebSocket audio streaming
- MQTT protocol support
