#ifndef PINS_H
#define PINS_H

/*
 * GPIO Pin Configuration for ESP32-S3
 */

// ==================== I2S Microphone (INMP441) ====================
#define I2S_MIC_NUM         0           // I2S0 for microphone
#define I2S_MIC_BCK         1           // Bit Clock
#define I2S_MIC_WS          2           // Word Select (LRCLK)
#define I2S_MIC_DATA        42          // Serial Data In

// ==================== I2S Speaker (MAX98357A) ====================
#define I2S_SPK_NUM         1           // I2S1 for speaker
#define I2S_SPK_BCK         15          // Bit Clock
#define I2S_SPK_WS          16          // Word Select (LRCLK)
#define I2S_SPK_DATA        17          // Serial Data Out

// ==================== I2C Display (OLED/LCD) ====================
#define I2C_SDA             8           // Data line
#define I2C_SCL             9           // Clock line
#define I2C_ADDRESS         0x3C        // Default OLED address

// ==================== LED Control ====================
#define LED_PIN             21          // Status LED

// ==================== Button ====================
#define BUTTON_PIN          40          // User button

// ==================== Power Management ====================
#define BATTERY_ADC_PIN     4           // Battery voltage sense
#define POWER_ENABLE_PIN    11          // Power enable control

// ==================== 4G Module (ML307) ====================
#define UART_4G_RX          20          // Serial RX
#define UART_4G_TX          19          // Serial TX
#define UART_4G_BAUD        115200

// ==================== Device Control ====================
#define SERVO_PIN           6           // PWM for servo
#define GPIO_RELAY1         7           // Relay output 1
#define GPIO_RELAY2         5           // Relay output 2

// ==================== Optional Pins ====================
#define GPIO_SPARE1         12
#define GPIO_SPARE2         13
#define GPIO_SPARE3         14
#define GPIO_SPARE4         18

/*
 * I2S Pin Configuration Structure
 */
struct I2SPinConfig {
  int bck;   // Bit Clock
  int ws;    // Word Select
  int data;  // Data pin
};

#endif // PINS_H
