#ifndef CONFIG_H
#define CONFIG_H

/*
 * ESP32-S3 AI Voice Assistant Configuration
 */

// ==================== WiFi Configuration ====================
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// ==================== Backend Configuration ====================
#define BACKEND_HOST "192.168.1.100"
#define BACKEND_PORT 5000
#define BACKEND_PROTOCOL "websocket"  // "websocket" or "mqtt"

// ==================== MQTT Configuration ====================
#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_USERNAME "your_username"
#define MQTT_PASSWORD "your_password"
#define MQTT_CLIENT_ID "esp32_voice_assistant"

// ==================== Audio Configuration ====================
#define SAMPLE_RATE 16000        // 16kHz sampling rate
#define BUFFER_SIZE 512          // Audio buffer size
#define AUDIO_CHANNELS 1         // Mono
#define BIT_DEPTH 16             // 16-bit audio
#define OPUS_BITRATE 16000       // OPUS codec bitrate

// ==================== Wake Word Configuration ====================
#define WAKE_WORD "alexa"        // Wake word to detect
#define WAKE_WORD_SENSITIVITY 0.7 // 0.0 to 1.0
#define WAKE_WORD_MODEL "multi_recognition" // ESP-SR model

// ==================== Display Configuration ====================
#define DISPLAY_TYPE "OLED"      // "OLED" or "LCD"
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define I2C_FREQ 400000          // 400kHz I2C frequency

// ==================== Power Management ====================
#define ENABLE_BATTERY_MONITORING true
#define BATTERY_LOW_THRESHOLD 20  // Percentage
#define BATTERY_CRITICAL_THRESHOLD 5

// ==================== Language Configuration ====================
#define LANGUAGE "english"       // "english", "bangla", "hindi"
#define TIMEZONE "UTC+6"

// ==================== Debug Configuration ====================
#define DEBUG_ENABLED true
#define DEBUG_SERIAL_BAUD 115200
#define DEBUG_LEVEL 2            // 0: Error, 1: Warn, 2: Info

// ==================== Device Configuration ====================
#define DEVICE_ID "ESP32S3_001"
#define DEVICE_NAME "AI Voice Assistant"
#define FIRMWARE_VERSION "1.0.0"

#endif // CONFIG_H
