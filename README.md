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
