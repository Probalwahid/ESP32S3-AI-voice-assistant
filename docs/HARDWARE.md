# Hardware Configuration Guide

Detailed hardware setup and pinout information.

## Supported Boards

- **ESP32-S3 Dev Kit** (Recommended)
- **ESP32-S3-DevKitC-1**
- **ESP32-S3-Box** (with modifications)

## Audio Hardware

### Microphone: INMP441

**Specifications**:
- Protocol: I2S (Inter-IC Sound)
- Sample Rate: 8kHz to 48kHz
- Bit Depth: 24-bit
- Channels: Mono
- Supply Voltage: 3.3V
- Current Consumption: ~0.5mA

**Pinout**:
```
VCC  ----→ 3.3V
GND  ----→ GND
SCK  ----→ GPIO 1 (I2S0 BCK)
WS   ----→ GPIO 2 (I2S0 WS)
SD   ----→ GPIO 42 (I2S0 DATA_IN)
```

### Speaker: MAX98357A

**Specifications**:
- Protocol: I2S (Inter-IC Sound)
- Output Power: 1W at 5V
- Supply Voltage: 3.3V-5.5V (recommended 5V)
- Channel: Mono Class D Amplifier
- Current Consumption: ~10mA (idle), ~100mA (playing)

**Pinout**:
```
VCC  ----→ 5V
GND  ----→ GND
BCLK ----→ GPIO 15 (I2S1 BCK)
LRCL ----→ GPIO 16 (I2S1 WS)
DIN  ----→ GPIO 17 (I2S1 DATA_OUT)
```

## Display Hardware

### OLED Display (SSD1306)

**Specifications**:
- Resolution: 128 x 64 pixels
- Protocol: I2C
- Supply Voltage: 3.3V
- Current Consumption: ~20mA

**Pinout**:
```
VCC ----→ 3.3V
GND ----→ GND
SCL ----→ GPIO 9
SDA ----→ GPIO 8
```

### LCD Display (HD44780)

**Alternative** if using character LCD:
- Protocol: I2C (via PCF8574 expander)
- Address: 0x27 (default)

## Connectivity Modules

### ML307 4G Module

**Pinout**:
```
RX  ----→ GPIO 20
TX  ----→ GPIO 19
GND ----→ GND
VCC ----→ 3.3V
```

**Configuration**:
- Baud Rate: 115200
- UART: UART1

## Power Management

### Battery Connection

**GPIO Configuration**:
- ADC Input: GPIO 4 (Battery Voltage Sense)
- Power Enable: GPIO 11 (Soft Power Control)

**Battery Voltage Monitoring**:
```
Battery +   ----→ Voltage Divider (4.2V → 3.3V)
Voltage Divider Output ----→ GPIO 4 (ADC)
Battery -   ----→ GND
```

### Power Budget

| Component | Current (mA) | Notes |
|-----------|-------------|-------|
| ESP32-S3  | 100-200     | Varies with activity |
| INMP441   | 0.5         | Microphone |
| MAX98357A | 10-100      | Idle to max output |
| OLED      | 20          | Display |
| WiFi      | 80-150      | Connected mode |
| **Total** | ~350-700    | Normal operation |

**Recommended Battery**: 
- Capacity: 3000-5000 mAh
- Chemistry: Li-Po or Li-ion
- Voltage: 3.7V-4.2V nominal

## GPIO Pin Summary

| GPIO | Function | I2S | I2C | Notes |
|------|----------|-----|-----|-------|
| 1    | I2S0 BCK | ✓   |     | Microphone clock |
| 2    | I2S0 WS  | ✓   |     | Microphone word select |
| 4    | Battery ADC |  |     | Battery voltage sense |
| 5    | GPIO_RELAY2 |  |     | Optional relay |
| 6    | Servo PWM |   |     | Optional servo control |
| 7    | GPIO_RELAY1 |  |     | Optional relay |
| 8    | I2C SDA  |     | ✓   | Display data |
| 9    | I2C SCL  |     | ✓   | Display clock |
| 11   | Power EN |     |     | Power enable |
| 15   | I2S1 BCK | ✓   |     | Speaker clock |
| 16   | I2S1 WS  | ✓   |     | Speaker word select |
| 17   | I2S1 DATA| ✓   |     | Speaker data |
| 19   | 4G TX   |     |     | Serial TX |
| 20   | 4G RX   |     |     | Serial RX |
| 21   | LED     |     |     | Status indicator |
| 40   | Button  |     |     | User input |
| 42   | I2S0 DATA | ✓  |     | Microphone data |

## I2S Configuration

### I2S0 (Microphone Input)
- **Mode**: I2S_PHILIPS_MODE
- **Sample Rate**: 16000 Hz
- **Bit Width**: 32-bit
- **Channels**: 1 (Mono)
- **Clock Source**: Internal

### I2S1 (Speaker Output)
- **Mode**: I2S_PHILIPS_MODE
- **Sample Rate**: 16000 Hz
- **Bit Width**: 16-bit
- **Channels**: 1 (Mono)
- **Clock Source**: Internal

## Troubleshooting

### No Audio Input
1. Check INMP441 power supply (3.3V)
2. Verify I2S pins are connected correctly
3. Check I2S0 configuration in firmware

### No Audio Output
1. Check MAX98357A power supply (5V)
2. Verify speaker connections
3. Test with known working speaker
4. Check I2S1 configuration

### Display Not Working
1. Verify I2C address (default: 0x3C)
2. Check I2C pull-up resistors
3. Check SDA/SCL connections
4. Use I2C scanner to detect device

### Power Issues
1. Check battery voltage (3.7-4.2V)
2. Verify power connection to ESP32-S3
3. Monitor current draw with multimeter

## Component Sourcing

Recommended suppliers:
- **AliExpress**: Budget-friendly options
- **Adafruit**: Quality components with documentation
- **Sparkfun**: Pre-assembled modules
- **DFRobot**: Verified components

## Schematic Diagram

A detailed schematic is available in the `docs/schematic/` directory showing:
- I2S audio connections
- I2C display connections
- Power distribution
- Optional components (4G, battery monitoring)

