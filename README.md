# BresserWeatherSensorTTN

[![CI](https://github.com/matthias-bs/BresserWeatherSensorTTN/actions/workflows/CI.yml/badge.svg)](https://github.com/matthias-bs/BresserWeatherSensorTTN/actions/workflows/CI.yml)
[![GitHub release](https://img.shields.io/github/release/matthias-bs/BresserWeatherSensorTTN?maxAge=3600)](https://github.com/matthias-bs/BresserWeatherSensorTTN/releases)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/LICENSE)

Bresser 5-in-1/6-in-1 868 MHz Weather Sensor Radio Receiver based on ESP32 and RFM95W/SX1276 - sends data to a LoRaWAN Network (e.g. The Things Network)

The RFM95W/SX1276 radio transceiver is used in FSK mode to receive weather sensor data and in LoRaWAN mode to connect to a LoRaWAN Network.

## Features
* Single 868 MHz Radio Transceiver for both Sensor Data Reception and LoRaWAN Connection
* Tested with The Things Network and Helium Network (EU868)
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
| BresserWeatherSensorReceiver     | r                            | https://github.com/matthias-bs/BresserWeatherSensorReceiver<br>=> add directory to BresserWeatherSensorTTN/src                                          |
| ESP32AnalogRead                  | o                            | - |
| OneWireNg                        | o                            | - |
| DallasTemperature                | o                            | - |
| ATC MiThermometer Library        | o                            | - |

Installation: "-" means normal Arduino IDE Library Installer 

See [dependencies](https://github.com/matthias-bs/BresserWeatherSensorTTN/network/dependencies) for required/tested versions.

## MQTT Integration

### The Things Network MQTT Integration and Message Decoder

Decode payload (a sequence of bytes) into data structures which are readable/suitable for further processing:
paste [ttn_decoder_fp.js](ttn_decoder_fp.js) as "Custom Javascript formatter" to "Payload Formatters" -> "Uplink" on The Things Network Console.

**Note:** The actual payload depends on the options selected in the Arduino software - the decoder must be edited accordingly (add or remove data types and JSON identifiers - see [ttn_decoder_fp.js line 176ff](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/f4de8c490c6f6ef72890b3b807953450cb171b35/ttn_decoder_fp.js#L210)).

TTN provides an MQTT broker.
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


JSON-Path with Uplink-Decoder (see [ttn_decoder_fp.js](ttn_decoder_fp.js))

`.uplink_message.decoded_payload.bytes.<variable>`

### Helium Network MQTT Integration and Message Decoder

Please refer to https://docs.helium.com/use-the-network/console/integrations/mqtt/.

Add an MQTT integration in the Helium console - the "Endpoint" is in fact an MQTT broker you have to provide:
![Helium_MQTT_Integration](https://user-images.githubusercontent.com/83612361/195050719-8562ad0e-5523-436f-8b61-e4b15b08d6de.png)

Add [helium_decoder.js](helium_decoder.js) in the Helium console as custom function:
![Helium_Decoder_Function](https://user-images.githubusercontent.com/83612361/195045593-d6c76e0c-1d87-410a-b941-8636b35d601a.png)

**Note:** The actual payload depends on the options selected in the Arduino software - the decoder must be edited accordingly (add or remove data types and JSON identifiers).

Add your function to the flow:
![Helium_Decoder_Flow](https://user-images.githubusercontent.com/83612361/195047042-6a8d9dfe-61f6-43e3-ac51-b917d01ff237.png)

Example decoder output (JSON):
```
rx = {[...],"decoded":{"payload":{"air_temp_c":"13.5","battery_v":4197,"humidity":72,"indoor_humidity":53,"indoor_temp_c":"21.7","rain_day":"0.0","rain_hr":"0.0","rain_mm":"480.8","rain_mon":"0.0","rain_week":"0.0","soil_moisture":0,"soil_temp_c":"-30.0",
"status":{"ble_ok":true,"res":false,"rtc_sync_req":true,"runtime_expired":true,"s1_batt_ok":false,"s1_dec_ok":false,"ws_batt_ok":true,"ws_dec_ok":true},
"supply_v":4210,"water_temp_c":"-30.0","wind_avg_meter_sec":"1.9","wind_direction_deg":"282.0","wind_gust_meter_sec":"2.0"},
"status":"success"}, [...]
```

## Doxygen Generated Source Code Documentation

https://matthias-bs.github.io/BresserWeatherSensorTTN/index.html

## References

Based on
* [BresserWeatherSensorReceiver](https://github.com/matthias-bs/BresserWeatherSensorReceiver) by Matthias Prinke
* [RadioLib](https://github.com/jgromes/RadioLib) by Jan Gromeš
* [MCCI LoRaWAN LMIC library](https://github.com/mcci-catena/arduino-lmic) by Thomas Telkamp and Matthijs Kooijman / Terry Moore, MCCI
* [MCCI Arduino LoRaWAN Library](https://github.com/mcci-catena/arduino-lorawan) by Terry Moore, MCCI
* [Lora-Serialization](https://github.com/thesolarnomad/lora-serialization) by Joscha Feth
* [ESP32Time](https://github.com/fbiego/ESP32Time) by Felix Biego
* [ESP32AnalogRead](https://github.com/madhephaestus/ESP32AnalogRead) by Kevin Harrington (madhephaestus)
* [OneWireNg](https://github.com/pstolarz/OneWireNg) by Piotr Stolarz
* [DallasTemperature / Arduino-Temperature-Control-Library](https://github.com/milesburton/Arduino-Temperature-Control-Library) by Miles Burton 
