#ifndef __CONFIG_H
#define __CONFIG_H 1

//
// Set up these variables to define behavior
//  \/                                 \/

#define WIFI_SSID                "SSID"
#define WIFI_PASSWORD            "password"
#define WIFI_HTTP_PORT           80

#define DEBUG                    true
#define USE_FAHRENHEIT           1              // comment this out to use celsius for everything
#define LCD_REFRESH_RATE         250            // refresh rate in ms
#define CONTROL_REFRESH_RATE     5000           // minimum switching time in ms for heat/humidity controls

#define TEMP_INCREMENT           0.5F
#define HUMIDITY_INCREMENT       0.5F
#define PUMP_OFFSET_MS           500

// --------------------------------------------------------

#define VERSION                  "0.1"

#define DHT22_SENSOR_PIN_NUMBER  19             // GPIO pin for DHT22 sensor
#define LCD_I2C_ADDRESS          0x27           // I2C bus address

#define GPIO_HEATER              4              // GPIO pin controlling heater      (#1)
#define GPIO_HEATER_FAN          16             // GPIO pin controlling heater fan  (#2)
#define GPIO_HUMIDIFIER          17             // GPIO pin controlling humidifier  (#3)
#define GPIO_PUMP                18             // GPIO pin controlling water pump  (#4)

#define GPIO_BUTTON0             12             // SELECT
#define GPIO_BUTTON1             14             // DEC
#define GPIO_BUTTON2             27             // INC
#define GPIO_BUTTON3             26             // BEFORE
#define GPIO_BUTTON4             25             // NEXT

#endif