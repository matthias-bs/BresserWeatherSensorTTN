# BresserWeatherSensorTTN
Bresser 5-in-1/6-in-1 868 MHz Weather Sensor Radio Receiver based on ESP32 and RFM95W - provides data via LoRaWAN to The Things Network

The RFM95W radio transceiver is used in FSK mode to receive weather sensor data and in LoRaWAN mode to connect to The Things Network.

## Features
* Single 868 MHz radio transceiver for both sensor data reception and LoRaWAN connection
* Low power design (using ESP32 deep sleep mode)
* Fast LoRaWAN joining after deep sleep (using ESP32 RTC RAM)
* ATC MiThermometer Bluetooth Low Energy Thernmometer/Hygrometer integration (optional)
* OneWire Temperature Sensor integration (optional)
* ESP32 Analog Digital Converter integration (optional)

## Hardware
(Hardware design coming soon...)

### ESP32 Module
[DFRobot FireBeetle ESP32 IoT](https://www.dfrobot.com/product-1590.html) (DFR0478) recomended due to its good low power design (but other boards will work, too)

### RFM95W- or SX1276-based Radio Transceiver Ḿodule

* [Adafruit RFM95W LoRa Radio Transceiver Breakout](https://www.adafruit.com/product/3072) (ADA3072) - 868/915 MHz version (868 MHz is used for both LoRaWAN and Bresser Weather Sensor in Europe)
* RF connector (u.FL or SMA as desired)
* Antenna ([Delock 89769 - LoRa 868 MHz Antenna SMA plug 3 dBi omnidirectional](https://www.delock.de/produkt/89769/merkmale.html?setLanguage=en) has been used with good results)

### Power Supply
Mains adapter or Li-Ion battery (with or without solar charger) - depending on desired operation time and duty cycle.


## Library Dependencies (Tested Versions)

| Library | Version | r: required /<br>o: optional | Installation |
| ------- | ------- | --------------- | ------------ |
| MCCI Arduino Development Kit ADK |  0.2.2 | r | - |
| MCCI LoRaWAN LMIC library        |  4.1.1 | r | - |
| MCCI Arduino LoRaWAN Library     |  0.9.2 | r | - |
| RadioLib                         |  5.2.0 | r | - |
| LoRa_Serialization               | LATEST | r | https://github.com/matthias-bs/lora-serialization<br>forked from https://github.com/thesolarnomad/lora-serialization LATEST<br>=> included with BresserWeatherSensorTTN/src |
| BresserWeatherSensorReceiver     |  0.0.3 | r | https://github.com/matthias-bs/BresserWeatherSensorReceiver<br>=> add directory to BresserWeatherSensorTTN/src                                          |
| ESP32AnalogRead                  |  0.2.0 | o | - |
| OneWire                          |  2.3.7 | o | - |
| DallasTemperature                |  3.9.0 | o | - |
| ATC MiThermometer Library        |  0.0.1 | o | - |

Installation: "-" means normal Arduino IDE Library Installer 

## Hardware

* ESP32 

## References

Based on
* [BresserWeatherSensorReceiver](https://github.com/matthias-bs/BresserWeatherSensorReceiver) by Matthias Prinke
* [RadioLib](https://github.com/jgromes/RadioLib) by Jan Gromeš
* [MCCI LoRaWAN LMIC library](https://github.com/mcci-catena/arduino-lmic) by Thomas Telkamp and Matthijs Kooijman / Terry Moore, MCCI
* [MCCI Arduino LoRaWAN Library](https://github.com/mcci-catena/arduino-lorawan) by Terry Moore, MCCI
* [Lora-Serialization](https://github.com/thesolarnomad/lora-serialization) by Joscha Feth (forked)
* [ESP32AnalogRead] by Kevin Harrington (madhephaestus) (https://github.com/madhephaestus/ESP32AnalogRead)
* [OneWire](https://github.com/PaulStoffregen/OneWire) - for Arduino by Paul Stoffregen
* [DallasTemperature / Arduino-Temperature-Control-Library](https://github.com/milesburton/Arduino-Temperature-Control-Library) by Miles Burton 
