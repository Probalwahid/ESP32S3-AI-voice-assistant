# ESP32-S3 AI Voice Assistant - Setup Guide

Complete setup instructions for the AI voice assistant project.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Hardware Setup](#hardware-setup)
3. [Firmware Installation](#firmware-installation)
4. [Backend Setup](#backend-setup)
5. [Web UI Setup](#web-ui-setup)
6. [Configuration](#configuration)
7. [Deployment](#deployment)

## Prerequisites

### Required Software
- Arduino IDE 2.0+ ([Download](https://www.arduino.cc/en/software))
- Python 3.9+ ([Download](https://www.python.org/downloads/))
- Node.js 16+ ([Download](https://nodejs.org/))
- Git ([Download](https://git-scm.com/))

### Required Hardware
- ESP32-S3 Development Board
- INMP441 I2S Digital Microphone
- MAX98357A I2S DAC Amplifier
- OLED or LCD Display (128x64)
- Breadboard & Jumper wires
- USB-C cable for ESP32-S3

## Hardware Setup

### Step 1: Connect Microphone (INMP441)

| INMP441 | ESP32-S3 |
|---------|----------|
| VCC     | 3.3V     |
| GND     | GND      |
| SCK     | GPIO 1   |
| WS      | GPIO 2   |
| SD      | GPIO 42  |

### Step 2: Connect Speaker (MAX98357A)

| MAX98357A | ESP32-S3 |
|-----------|----------|
| VCC       | 5V       |
| GND       | GND      |
| BCLK      | GPIO 15  |
| LRCL      | GPIO 16  |
| DIN       | GPIO 17  |

### Step 3: Connect Display (OLED)

| OLED | ESP32-S3 |
|------|----------|
| VCC  | 3.3V     |
| GND  | GND      |
| SCL  | GPIO 9   |
| SDA  | GPIO 8   |

### Step 4: Connect Power

- Use USB-C cable to power ESP32-S3
- For battery operation, connect to GPIO 11 (Power Enable) and GPIO 4 (Battery Sense)

## Firmware Installation

### Step 1: Clone Repository

```bash
git clone https://github.com/Probalwahid/ESP32S3-AI-voice-assistant.git
cd ESP32S3-AI-voice-assistant
```

### Step 2: Setup Arduino IDE

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add this board manager URL:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Board Manager**
5. Search and install "esp32 by Espressif Systems"
6. Select **Board: ESP32S3 Dev Module**

### Step 3: Install Libraries

In Arduino IDE, go to **Sketch → Include Library → Manage Libraries**:

- I2S
- ArduinoJson
- WiFi (built-in)
- WebSocket
- PubSubClient (for MQTT)

### Step 4: Configure and Upload

1. Open `firmware/ESP32_S3_Voice_Assistant.ino`
2. Edit `firmware/config/config.h` with your WiFi credentials:
   ```cpp
   #define WIFI_SSID "YOUR_SSID"
   #define WIFI_PASSWORD "YOUR_PASSWORD"
   ```
3. Edit `firmware/config/pins.h` if using different GPIO pins
4. Connect ESP32-S3 via USB
5. Select correct COM port: **Tools → Port**
6. Click **Upload**

### Step 5: Monitor Serial Output

Open **Tools → Serial Monitor** (115200 baud) to see startup messages.

## Backend Setup

### Step 1: Create Virtual Environment

```bash
cd backend
python -m venv venv

# On Windows
venv\Scripts\activate

# On macOS/Linux
source venv/bin/activate
```

### Step 2: Install Dependencies

```bash
pip install -r requirements.txt
```

### Step 3: Configure Environment

Copy `.env.example` to `.env` and update values:

```bash
cp .env.example .env
```

Edit `.env`:
```
DEBUG=True
HOST=0.0.0.0
PORT=5000
LLM_API_KEY=your_api_key_here
```

### Step 4: Run Backend

```bash
python app.py
```

The backend will start at `http://localhost:5000`

## Web UI Setup

### Step 1: Install Dependencies

```bash
cd web-ui
npm install
```

### Step 2: Development Server

```bash
npm run dev
```

Access at `http://localhost:3000`

### Step 3: Build for Production

```bash
npm run build
```

Production files will be in `web-ui/dist/`

## Configuration

### WiFi Configuration

Edit `firmware/config/config.h`:
```cpp
#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"
```

### Backend URL Configuration

Update in firmware:
```cpp
#define BACKEND_HOST "192.168.1.100"
#define BACKEND_PORT 5000
```

### MQTT Configuration (Optional)

If using MQTT protocol:
```cpp
#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "password"
```

### Language Configuration

```cpp
#define LANGUAGE "english"  // "english", "bangla", "hindi"
```

## Deployment

### Docker Deployment (Optional)

Build and run with Docker:

```bash
docker-compose up -d
```

This starts:
- Backend on port 5000
- Web UI on port 3000
- MQTT Broker on port 1883

### Production Deployment

1. **Backend**:
   - Use Gunicorn/uWSGI
   - Setup Nginx reverse proxy
   - Enable HTTPS/SSL

2. **Web UI**:
   - Build static files
   - Serve from Nginx
   - Configure for production

3. **Firmware**:
   - Build release binary
   - Test on multiple devices
   - Setup OTA updates

## Troubleshooting

See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for common issues and solutions.

## Next Steps

- Configure your specific wake word
- Setup LLM API keys (OpenAI, etc.)
- Train speaker recognition models
- Customize UI and assets

