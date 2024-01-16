# INCUBATOR

ESP32 based Wildlife Rehabilitation incubator assembly. 

Designed to accommodate neonatal and juvenile opossums, but can be used for other animals.

Features:
 - LCD display
 - Heat and humidity controls using 12V convection heating core and ultrasonic humidifier assembly
 - Low build cost
 - Easy to source parts

Arduino Libraries:
 - [LiquidCrystalIO](https://www.arduino.cc/reference/en/libraries/liquidcrystal/)
 - [DHT-Sensors-Non-Blocking](https://github.com/toannv17/DHT-Sensors-Non-Blocking)

Components:
 - [ESP32-WROOM-32 microcontroller](https://www.amazon.com/gp/product/B08D5ZD528/) - $16 for 3
 - [I2C 2004 20x4 LCD display](https://www.amazon.com/gp/product/B0C1G9GBRZ/) - $16 for 3
 - [100W 12V heating assembly](https://www.amazon.com/gp/product/B08YX4ZSFX/) - $21
 - [5V humidifier oscillator circuit](https://www.amazon.com/gp/product/B0C1C79BH3/) - $10 for 3
 - [DHT22 Digital Temperature and Humidity Module](https://www.amazon.com/gp/product/B08RYDMDJF/) - $19 for 5
 - [Four channel relay](https://www.amazon.com/gp/product/B0CJ6MYZ74/) - $8
 - [Buck converters](https://www.amazon.com/gp/product/B01MQGMOKI/) - $10 for 6

## General Notes

 - 2023-12-27 - I have been unit testing the code and assembly components one at a time to ensure that everything works properly before attempting a full build. The software appears to function correctly, although it may need to be tweaked depending on the rate at which the heater and humidifier work. 

## Compilation

To compile/install, run `pio run -t upload` with an ESP32 board plugged in. All configuration is set in `src/config.h`.

## Build Notes

_For this build to work properly, you will also need some sort of enclosure to retain the heat and humidity. An old non-working wine fridge or something similar is probably the easiest._

 - Caveat: Did not include the cost of any interconnecting wires, solder, etc. Builds are builds.
 - The poles on the switch for the humidifier circuit will need to be soldered together so that the switch can be operated in an automated fashion.

### Wiring

* LCD SCL -> ESP32 D22
* LCD SDA -> ESP32 D21
* LCD VCC -> ESP32 V1N (5V)
* LCD GND -> ESP32 GND

* DHT22 OUT -> ESP32 D19
* DHT22 + -> ESP32 3V3 (3.3V)
* DHT22 - -> ESP32 GND

* PAD 1 (TOP) -> ESP32 D25
* PAD 2 -> ESP32 D27
* PAD 3 -> ESP32 D26
* PAD 4 -> ESP32 D14
* PAD 5 -> ESP32 D20
* PAD 6 (BOTTOM) -> ESP32 GND

* RELAY GND -> ESP32 GND
* RELAY VCC -> ESP32 V1N (5V) (should come directly from power source)
* RELAY IN1 -> ESP32 D4
* RELAY IN2 -> ESP32 RX2
* RELAY IN3 -> ESP32 TX2
* RELAY IN4 -> ESP32 D18

## TODO

- [ ] Publish circuit diagram
- [ ] Determine maximum amperage draw for 12V power supply
- [ ] Design mechanical assembly for providing water to humidifying circuit
- [ ] Pictures

## References

- [Custom Icon Generator](https://maxpromer.github.io/LCD-Character-Creator/)

