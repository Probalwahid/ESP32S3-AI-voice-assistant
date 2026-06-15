# Troubleshooting Guide

Common issues and solutions for the ESP32-S3 AI Voice Assistant.

## Table of Contents

1. [Firmware Issues](#firmware-issues)
2. [Audio Problems](#audio-problems)
3. [Communication Issues](#communication-issues)
4. [Backend Problems](#backend-problems)
5. [Hardware Issues](#hardware-issues)
6. [Performance Issues](#performance-issues)

## Firmware Issues

### Problem: Upload Failed

**Error**: `Failed to connect to ESP32-S3. Could not open COM port.`

**Solutions**:
1. Check USB cable connection
2. Install CH340 or CP2102 driver (depending on your board)
3. Select correct COM port in Arduino IDE
4. Try different USB port on computer
5. Reset the board by holding BOOT button while plugging in

### Problem: Compilation Error

**Error**: `'I2S' was not declared in this scope`

**Solutions**:
1. Verify ESP32 board package is installed (v2.0+)
2. Select correct board: **ESP32S3 Dev Module**
3. Update board package: **Boards Manager → Search "esp32" → Update**
4. Restart Arduino IDE

### Problem: Memory Error

**Error**: `Not enough memory`

**Solutions**:
1. Remove unused libraries
2. Use PSRAM if available
3. Reduce audio buffer size in config.h
4. Enable optimization: **Tools → Optimization → O3**

## Audio Problems

### Problem: No Audio Input (Microphone Not Working)

**Symptoms**: Serial monitor shows no audio data, microphone seems dead

**Troubleshooting Steps**:

1. **Check Power Supply**
   ```cpp
   // Add this to setup() to test microphone power
   digitalWrite(POWER_ENABLE_PIN, HIGH);
   delay(100);
   Serial.println("Microphone power enabled");
   ```

2. **Verify I2S Connection**
   - Check GPIO connections (1, 2, 42)
   - Verify wiring matches `firmware/config/pins.h`
   - Test with multimeter

3. **Test I2S Interface**
   ```cpp
   void testI2S() {
     int32_t sampleBuffer[512];
     int bytesRead = I2S.read((char*)sampleBuffer, sizeof(sampleBuffer));
     
     if (bytesRead > 0) {
       Serial.println("I2S working");
     } else {
       Serial.println("I2S not receiving data");
     }
   }
   ```

4. **Check Microphone Sensitivity**
   - Try high volume source (speaker, clapping)
   - Adjust wake word sensitivity in config.h

### Problem: No Audio Output (Speaker Not Working)

**Symptoms**: Speaker makes no sound, no LED indication

**Troubleshooting Steps**:

1. **Check Power Supply**
   - Verify 5V supply to MAX98357A
   - Check power indicator on amplifier
   - Use multimeter to verify voltage

2. **Test Speaker Connection**
   - Connect test speaker
   - Verify GPIO 15, 16, 17 connections
   - Check volume setting

3. **Test Audio Output**
   ```cpp
   void testSpeaker() {
     // Generate test tone
     int16_t testBuffer[1024];
     for(int i = 0; i < 1024; i++) {
       testBuffer[i] = sin(i * 0.01) * 32767;
     }
     I2S1.write((char*)testBuffer, sizeof(testBuffer));
   }
   ```

4. **Check I2S1 Configuration**
   - Verify I2S1 is initialized in setup()
   - Check GPIO assignments

### Problem: Distorted Audio

**Symptoms**: Audio output sounds crackling or distorted

**Solutions**:
1. Reduce audio buffer size
2. Lower sample rate to 8kHz
3. Increase CPU frequency
4. Check power supply stability
5. Reduce OPUS bitrate

## Communication Issues

### Problem: WiFi Connection Failed

**Error**: `WiFi connection timeout` or `Failed to connect to SSID`

**Solutions**:
1. Check SSID and password in config.h
2. Verify WiFi router is broadcasting 2.4GHz (ESP32-S3 doesn't support 5GHz)
3. Check signal strength
4. Restart router
5. Update WiFi credentials without changing other code

### Problem: Backend Connection Failed

**Error**: `Connection refused` or `Network unreachable`

**Solutions**:
1. Verify backend server is running
   ```bash
   cd backend
   python app.py
   ```

2. Check backend URL in firmware config
   ```cpp
   #define BACKEND_HOST "192.168.x.x"
   #define BACKEND_PORT 5000
   ```

3. Test connectivity from ESP32
   ```cpp
   void testConnection() {
     WiFiClient client;
     if (client.connect(BACKEND_HOST, BACKEND_PORT)) {
       Serial.println("Connected to backend");
     } else {
       Serial.println("Connection failed");
     }
   }
   ```

4. Check firewall settings
5. Ensure devices are on same network

### Problem: MQTT Connection Issues

**Error**: `MQTT connection failed` or `Broker not reachable`

**Solutions**:
1. Verify MQTT broker is running
   ```bash
   # Using Eclipse Mosquitto
   mosquitto -c mosquitto.conf
   ```

2. Check MQTT broker address and port
3. Verify credentials if authentication is enabled
4. Check network connectivity to broker
5. Use MQTT test client to verify broker

## Backend Problems

### Problem: Backend Server Won't Start

**Error**: `Address already in use` or `Connection refused`

**Solutions**:
1. Check if port is already in use
   ```bash
   # Linux/Mac
   lsof -i :5000
   
   # Windows
   netstat -ano | findstr :5000
   ```

2. Kill process using port or use different port
3. Check Python version (3.9+)
4. Verify virtual environment is activated

### Problem: Import Errors

**Error**: `ModuleNotFoundError: No module named 'flask'`

**Solutions**:
1. Verify virtual environment is activated
2. Reinstall dependencies
   ```bash
   pip install -r requirements.txt
   ```

3. Check Python version compatibility

### Problem: API Endpoint Returns 500 Error

**Error**: `Internal Server Error`

**Solutions**:
1. Check backend logs for details
2. Verify audio format is correct
3. Check LLM API key is valid
4. Restart backend server

## Hardware Issues

### Problem: Display Not Working

**Symptoms**: OLED/LCD shows nothing or garbage

**Solutions**:
1. Check I2C connections (GPIO 8, 9)
2. Verify I2C address
   ```cpp
   void scanI2C() {
     for(int i=0; i<127; i++) {
       Wire.beginTransmission(i);
       if(Wire.endTransmission() == 0) {
         Serial.printf("I2C device found at 0x%02X\n", i);
       }
     }
   }
   ```

3. Check pull-up resistors (usually 4.7kΩ)
4. Verify display power supply
5. Try different I2C address

### Problem: Button Not Responsive

**Symptoms**: Button press not detected

**Solutions**:
1. Check GPIO 40 connection
2. Add debounce delay
3. Check pullup resistor
4. Test with multimeter

### Problem: Battery Monitoring Not Working

**Symptoms**: Battery percentage shows 0% or incorrect value

**Solutions**:
1. Check GPIO 4 (ADC) connection
2. Verify voltage divider resistors
3. Calibrate ADC reading
   ```cpp
   void calibrateBattery() {
     int raw = analogRead(BATTERY_ADC_PIN);
     float voltage = raw * 3.3 / 4095 * 2; // Assuming 1:2 divider
     Serial.printf("Battery voltage: %.2fV\n", voltage);
   }
   ```

## Performance Issues

### Problem: High Latency

**Symptoms**: Response time is slow (>2 seconds)

**Solutions**:
1. Check network latency
   ```bash
   ping <backend-ip>
   ```

2. Reduce audio chunk size
3. Increase CPU frequency
4. Optimize LLM response time
5. Use WebSocket instead of MQTT

### Problem: High Memory Usage

**Symptoms**: Out of memory errors

**Solutions**:
1. Monitor free heap
   ```cpp
   Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
   ```

2. Reduce buffer sizes
3. Remove unused features
4. Use PSRAM for large buffers

### Problem: Intermittent Crashes

**Symptoms**: Device randomly reboots

**Solutions**:
1. Enable core debug level
2. Monitor watchdog timeout
3. Check for buffer overflows
4. Update to latest ESP32 core
5. Reduce CPU overclocking if applicable

## Debug Mode

### Enable Debug Logging

Set in `config.h`:
```cpp
#define DEBUG_ENABLED true
#define DEBUG_LEVEL 2  // 0: Error, 1: Warn, 2: Info
```

### Access Serial Monitor

1. Connect USB cable
2. Open Serial Monitor in Arduino IDE
3. Set baud rate to 115200
4. Monitor logs

### Enable Core Debug

In Arduino IDE:
- **Tools → Core Debug Level → Verbose**

## Advanced Troubleshooting

### Check System Logs

**Backend Logs**:
```bash
tail -f backend.log
```

**System Resources**:
```bash
# Check CPU and memory
top

# Check network
netstat -an | grep 5000
```

### Network Diagnostics

```bash
# Ping backend
ping <backend-ip>

# Check connectivity
curl http://<backend-ip>:5000/health

# Monitor traffic
tcpdump -i eth0 port 5000
```

### Hardware Diagnostics

Use multimeter to check:
- Power supply voltages
- Ground continuity
- I2C signal levels
- I2S clock frequency

## Getting Help

If you still can't solve the issue:

1. **Check Documentation**
   - Review [SETUP.md](SETUP.md)
   - Check [HARDWARE.md](HARDWARE.md)
   - Read [FIRMWARE.md](docs/FIRMWARE.md)

2. **Search Issues**
   - GitHub Issues: https://github.com/Probalwahid/ESP32S3-AI-voice-assistant/issues

3. **Create New Issue**
   - Include error message
   - Attach serial logs
   - Describe setup and steps to reproduce

4. **Community Support**
   - ESP32 Forum
   - Arduino Community
   - Stack Overflow

## Common Error Messages

| Error | Cause | Solution |
|-------|-------|----------|
| `Failed to connect` | WiFi/Backend unreachable | Check network connection |
| `Out of memory` | Insufficient RAM | Reduce buffer sizes |
| `I2S error` | Audio interface issue | Check GPIO connections |
| `MQTT timeout` | Broker unreachable | Verify broker address |
| `API 500 error` | Backend crash | Check backend logs |
| `Watchdog timeout` | Code stuck in loop | Enable debugging |

