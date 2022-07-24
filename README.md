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
