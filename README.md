# BresserWeatherSensorTTN

[![CI](https://github.com/matthias-bs/BresserWeatherSensorTTN/actions/workflows/CI.yml/badge.svg)](https://github.com/matthias-bs/BresserWeatherSensorTTN/actions/workflows/CI.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/LICENSE)

Bresser 5-in-1/6-in-1 868 MHz Weather Sensor Radio Receiver based on ESP32 and RFM95W - provides data via LoRaWAN to The Things Network

The RFM95W radio transceiver is used in FSK mode to receive weather sensor data and in LoRaWAN mode to connect to The Things Network.

## Features
* Single 868 MHz Radio Transceiver for both Sensor Data Reception and LoRaWAN Connection
* Supports multiple 868 MHz Sensors (e.g. Weather Sensor and Soil Moisture Sensor or Indoor Thermometer/Hygrometer)
* Low Power Design (using ESP32 Deep Sleep Mode)
* Fast LoRaWAN Joining after Deep Sleep (using ESP32 RTC RAM)
* ATC MiThermometer Bluetooth Low Energy Thermometer/Hygrometer Integration (optional)
* OneWire Temperature Sensor Integration (optional)
* ESP32 Analog Digital Converter Integration (optional)

## Hardware
Please see my [LoRaWAN_Node](https://github.com/matthias-bs/LoRaWAN_Node) project.

### ESP32 Module
[DFRobot FireBeetle ESP32 IoT](https://www.dfrobot.com/product-1590.html) (DFR0478) recomended due to its good low power design (but other boards will work, too)

### RFM95W- or SX1276-based Radio Transceiver Module

* [Adafruit RFM95W LoRa Radio Transceiver Breakout](https://www.adafruit.com/product/3072) (ADA3072) - 868/915 MHz version (868 MHz is used for both LoRaWAN and Bresser Weather Sensor in Europe)
* RF connector (u.FL or SMA as desired)
* Antenna ([Delock 89769 - LoRa 868 MHz Antenna SMA plug 3 dBi omnidirectional](https://www.delock.de/produkt/89769/merkmale.html?setLanguage=en) has been used with good results)

### Power Supply
Mains adapter or Li-Ion battery (with or without solar charger) - depending on desired operation time and duty cycle.

## Software Build Setup
TBD

### Library Dependencies

| Library                          | r: required /<br>o: optional | Installation |
| -------------------------------- | ---------------------------- | ------------ |
| MCCI Arduino Development Kit ADK | r                            | - |
| MCCI LoRaWAN LMIC library        | r                            | - |
| MCCI Arduino LoRaWAN Library     | r                            | - |
| RadioLib                         | r                            | - |
| LoRa_Serialization               | r                            | - |
| ESP32Time                        | r                            | - |
| Timezone                         | r                            | - |
| BresserWeatherSensorReceiver     | r                            | https://github.com/matthias-bs/BresserWeatherSensorReceiver<br>=> add directory to BresserWeatherSensorTTN/src                                          |
| ESP32AnalogRead                  | o                            | - |
| OneWire                          | o                            | - |
| DallasTemperature                | o                            | - |
| ATC MiThermometer Library        | o                            | - |

Installation: "-" means normal Arduino IDE Library Installer 

See [dependencies](https://github.com/matthias-bs/BresserWeatherSensorTTN/network/dependencies) for required/tested versions.

## The Things Network MQTT Integration

### The Things Network Decoder

Decode payload (a sequence of bytes) into data structures which are readable/suitable for further processing:
paste [ttn_decoder_fp_v6.js](ttn_decoder_fp_v6.js) as "Custom Javascript formatter" to "Payload Formatters" -> "Uplink" on The Things Network Console.
The actual payload depends on the options selected in the Arduino software - the decoder must be edited accordingly (add or remove data types and JSON identifiers - see [ttn_decoder_fp_v6.js line 176ff](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/f619c65d3c4df3991029d46554dc78ba7096d24f/ttn_decoder_fp_v6.js#L176)).

### TTN-MQTT-Integration

How to receive and decode the payload with an MQTT client -
see https://www.thethingsnetwork.org/forum/t/some-clarity-on-mqtt-topics/44226/2

V3 topic:

`v3/<ttn app id><at symbol>ttn/devices/<ttn device id>/up`

  
v3 message key field jsonpaths:
  
```
<ttn device id> = .end_device_ids.device_id
<ttn app id> = .end_device_ids.application_ids.application_id  // (not including the <at symbol>ttn in the topic)
<payload> = .uplink_message.frm_payload
```  


JSON-Path with Uplink-Decoder (see [ttn_decoder_fp_v6.js](ttn_decoder_fp_v6.js))

`.uplink_message.decoded_payload.bytes.<variable>`

## References

Based on
* [BresserWeatherSensorReceiver](https://github.com/matthias-bs/BresserWeatherSensorReceiver) by Matthias Prinke
* [RadioLib](https://github.com/jgromes/RadioLib) by Jan Gromeš
* [MCCI LoRaWAN LMIC library](https://github.com/mcci-catena/arduino-lmic) by Thomas Telkamp and Matthijs Kooijman / Terry Moore, MCCI
* [MCCI Arduino LoRaWAN Library](https://github.com/mcci-catena/arduino-lorawan) by Terry Moore, MCCI
* [Lora-Serialization](https://github.com/thesolarnomad/lora-serialization) by Joscha Feth
* [ESP32Time](https://github.com/fbiego/ESP32Time) by Felix Biego
* [Arduino Timezone Library](https://github.com/JChristensen/Timezone) by Jack Cristensen
* [ESP32AnalogRead](https://github.com/madhephaestus/ESP32AnalogRead) by Kevin Harrington (madhephaestus)
* [OneWire](https://github.com/PaulStoffregen/OneWire) for Arduino by Paul Stoffregen
* [DallasTemperature / Arduino-Temperature-Control-Library](https://github.com/milesburton/Arduino-Temperature-Control-Library) by Miles Burton 
