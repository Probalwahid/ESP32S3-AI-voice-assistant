# ESP32-S3 AI Voice Assistant

A comprehensive AI-powered voice assistant system running on ESP32-S3 with multi-protocol support, speaker recognition, and cloud integration.

## Features

### Hardware
- **Microcontroller**: ESP32-S3
- **Microphone**: INMP441 I2S Digital MEMS
- **Speaker**: MAX98357A I2S DAC Class D Mono Amplifier
- **Connectivity**: Wi-Fi / ML307 Cat.1 4G
- **Display**: OLED / LCD with emoji support
- **Power**: Battery with management system

### Software Features
- **Wake Word Detection**: Offline ESP-SR engine
- **Audio Codec**: OPUS compression
- **Communication**: WebSocket and MQTT+UDP protocols
- **AI Pipeline**: Streaming ASR → LLM → TTS
- **Speaker Recognition**: 3D Speaker identification
- **Device Control**: On-device MCP (LED, Servo, GPIO, Speaker)
- **Cloud Integration**: Cloud-side MCP for extended capabilities
- **Multi-Language**: English, Bangla, Hindi
- **Customization**: Web-based assets editor (wake words, fonts, emojis)

## Project Structure

```
ESP32S3-AI-voice-assistant/
├── firmware/                 # Arduino IDE firmware
│   ├── ESP32_S3_Voice_Assistant.ino
│   ├── config/
│   │   ├── config.h
│   │   ├── pins.h
│   │   ├── wifi_config.h
│   │   └── mqtt_config.h
│   ├── src/
│   │   ├── audio/           # Audio I/O (INMP441, MAX98357A)
│   │   ├── ml/              # ESP-SR wake word detection
│   │   ├── comms/           # WebSocket & MQTT handlers
│   │   ├── display/         # OLED/LCD driver
│   │   ├── power/           # Battery & power management
│   │   ├── mcp/             # Model Context Protocol
│   │   └── utils/           # Utility functions
│   └── lib/                 # Custom libraries
│
├── backend/                 # Backend server
│   ├── app.py              # Flask/FastAPI app
│   ├── config.py
│   ├── requirements.txt
│   ├── routes/
│   │   ├── audio.py        # ASR/TTS endpoints
│   │   ├── llm.py          # LLM interface
│   │   ├── mcp.py          # Cloud MCP handlers
│   │   └── speaker.py      # Speaker recognition
│   ├── models/
│   │   ├── asr.py          # ASR service
│   │   ├── tts.py          # TTS service
│   │   ├── llm.py          # LLM service
│   │   └── speaker_id.py   # Speaker recognition model
│   ├── services/
│   │   ├── opus_codec.py   # OPUS codec handling
│   │   ├── mqtt_handler.py # MQTT protocol
│   │   └── websocket_handler.py
│   └── utils/
│
├── web-ui/                 # Web dashboard & assets editor
│   ├── public/
│   │   ├── index.html
│   │   └── assets/
│   ├── src/
│   │   ├── components/
│   │   ├── pages/
│   │   │   ├── Dashboard.jsx
│   │   │   ├── ChatInterface.jsx
│   │   │   └── AssetsEditor.jsx
│   │   ├── services/
│   │   │   ├── api.js
│   │   │   └── websocket.js
│   │   └── App.jsx
│   ├── package.json
│   └── vite.config.js
│
├── docker/
│   ├── Dockerfile.backend
│   ├── docker-compose.yml
│   └── Dockerfile.web
│
├── docs/
│   ├── SETUP.md            # Setup guide
│   ├── HARDWARE.md         # Hardware guide
│   ├── FIRMWARE.md         # Firmware development
│   ├── BACKEND.md          # Backend setup
│   ├── API.md              # API documentation
│   ├── MCP.md              # MCP documentation
│   └── TROUBLESHOOTING.md
│
├── .github/
│   └── workflows/          # CI/CD pipelines
│       ├── firmware-check.yml
│       └── backend-test.yml
│
├── .gitignore
├── LICENSE
└── docker-compose.yml

```

## Quick Start

### Prerequisites
- Arduino IDE 2.0+
- Python 3.9+
- Node.js 16+
- ESP32 Arduino Board Package
- Docker (optional)

### Firmware Setup
1. Clone repository and open `firmware/ESP32_S3_Voice_Assistant.ino` in Arduino IDE
2. Install required board: ESP32-S3 by Espressif
3. Configure pins in `firmware/config/pins.h`
4. Install dependencies (OPUS codec, ESP-SR)
5. Upload to ESP32-S3

### Backend Setup
```bash
cd backend
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate
pip install -r requirements.txt
python app.py
```

### Web UI Setup
```bash
cd web-ui
npm install
npm run dev
```

## Hardware Connections

### INMP441 Microphone (I2S Input)
- **VCC** → 3.3V
- **GND** → GND
- **SCK** → GPIO 1
- **WS** → GPIO 2
- **SD** → GPIO 42

### MAX98357A Amplifier (I2S Output)
- **VCC** → 5V
- **GND** → GND
- **BCLK** → GPIO 15
- **LRCL** → GPIO 16
- **DIN** → GPIO 17

### OLED Display (I2C)
- **VCC** → 3.3V
- **GND** → GND
- **SCL** → GPIO 9
- **SDA** → GPIO 8

## API Documentation

### WebSocket Connection
```javascript
ws://device-ip:8080/audio
```

### MQTT Topics
- `device/audio/input` - Incoming audio stream
- `device/audio/output` - Output audio stream
- `device/control/led` - LED control
- `device/control/servo` - Servo control

## Configuration

Edit `firmware/config/config.h`:
```cpp
#define WIFI_SSID "Your_SSID"
#define WIFI_PASSWORD "Your_Password"
#define BACKEND_HOST "192.168.x.x"
#define BACKEND_PORT 5000
```

## Development

### Firmware Development
See [FIRMWARE.md](docs/FIRMWARE.md)

### Backend Development
See [BACKEND.md](docs/BACKEND.md)

### MCP Integration
See [MCP.md](docs/MCP.md)

## Testing

```bash
# Run backend tests
cd backend
pytest tests/

# Run firmware checks (if applicable)
cd firmware
# Arduino IDE compilation checks
```

## Troubleshooting

See [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see LICENSE file for details.

## Support

For issues and questions:
- Open a GitHub Issue
- Check Troubleshooting Guide
- Review Documentation

## Acknowledgments

- Espressif ESP32 & ESP-SR team
- Arduino community
- OPUS codec contributors
