# BresserWeatherSensorTTN

[![CI](https://github.com/matthias-bs/BresserWeatherSensorTTN/actions/workflows/CI.yml/badge.svg)](https://github.com/matthias-bs/BresserWeatherSensorTTN/actions/workflows/CI.yml)
[![GitHub release](https://img.shields.io/github/release/matthias-bs/BresserWeatherSensorTTN?maxAge=3600)](https://github.com/matthias-bs/BresserWeatherSensorTTN/releases)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/LICENSE)

Bresser 5-in-1/6-in-1/7-in-1 868 MHz Weather Sensor Radio Receiver based on ESP32 and RFM95W/SX1276 - sends data to a LoRaWAN Network (e.g. The Things Network)
Support for RP2040 (Arduino-Pico) has been added recently.

The RFM95W/SX1276 radio transceiver is used in FSK mode to receive weather sensor data and in LoRaWAN mode to connect to a LoRaWAN Network.

Auxiliary sensor data can be integrated via Bluetooth Low Energy (BLE), OneWire, UART, analog/digital inputs etc. 

## Features
* Single 868 MHz Radio Transceiver for both Sensor Data Reception and LoRaWAN Connection
* Tested with [The Things Network](https://www.thethingsnetwork.org/) and [Helium Network](https://www.helium.com/) (EU868)
* Supports multiple 868 MHz Sensors (e.g. Weather Sensor and Soil Moisture Sensor or Indoor Thermometer/Hygrometer)
* See [BresserWeatherSensorReceiver](https://github.com/matthias-bs/BresserWeatherSensorReceiver) for supported sensors
* Low Power Design (using ESP32 Deep Sleep Mode / RP2040 Sleep State)
* Fast LoRaWAN Joining after Deep Sleep (using ESP32 RTC RAM / RP2040 Flash)
* [ATC MiThermometer](https://github.com/pvvx/ATC_MiThermometer) Bluetooth Low Energy Thermometer/Hygrometer Integration (optional)
* [Theengs Decoder](https://github.com/theengs/decoder) Bluetooth Low Energy Sensors Integration (optional)
* OneWire Temperature Sensor Integration (optional)
* ESP32/RP2040 Analog Digital Converter Integration (optional)
* [A02YYUW / DFRobot SEN0311 Ultrasonic Distance Sensor](https://wiki.dfrobot.com/_A02YYUW_Waterproof_Ultrasonic_Sensor_SKU_SEN0311) (30...4500mm)<br>
  (optional, SW integrationnot yet available for RP2040)
* [Remote Configuration via LoRaWAN Downlink](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/README.md#remote-configuration-via-lorawan-downlink)

## Hardware
* [LoRaWAN_Node](https://github.com/matthias-bs/LoRaWAN_Node)
* [LILYGO® TTGO LORA32](https://www.lilygo.cc/products/lora3?variant=42272562282677)
    
    [TTGO LoRa32 V2.1.6 Pinout](https://github.com/lnlp/pinout-diagrams/blob/main/LoRa%20development%20boards/TTGO%20LoRa32%20V2.1.6%20Pinout%20(LLP).pdf) 

* [Heltec Wireless Stick](https://heltec.org/project/wireless-stick/)
* *Resumably* [Heltec WiFi LoRa32 V2](https://heltec.org/project/wifi-lora-32/) (confirmation wanted!!!)
* *Presumably* [Adafruit Feather ESP32-S2](https://www.adafruit.com/product/4769) with [Adafruit LoRa Radio FeatherWing](https://www.adafruit.com/product/3231) (confirmation wanted!!!)

* *Presumably* [Adafruit Feather ESP32](https://www.adafruit.com/product/3405) with [Adafruit LoRa Radio FeatherWing](https://www.adafruit.com/product/3231) (confirmation wanted!!!)

* [Thingpulse ePulse Feather](https://thingpulse.com/product/epulse-feather-low-power-esp32-development-board/) with [Adafruit LoRa Radio FeatherWing](https://www.adafruit.com/product/3231) (**see** [**#56**](https://github.com/matthias-bs/BresserWeatherSensorTTN/issues/56))

* [DFRobot FireBeetle ESP32 IoT Microcontroller](https://www.dfrobot.com/product-1590.html) with [FireBeetle Cover LoRa Radio 868MHz](https://www.dfrobot.com/product-1831.html)

* [Adafruit Feather RP2040](https://www.adafruit.com/product/4884) with [Adafruit LoRa Radio FeatherWing](https://www.adafruit.com/product/3231)

See [The Things Network's](https://www.thethingsnetwork.org) [Big ESP32 + SX127x topic part 2](https://www.thethingsnetwork.org/forum/t/big-esp32-sx127x-topic-part-2/11973) for some hardware options.

See [Leonel Lopes Parente's](https://github.com/lnlp) collection of [LoRa development boards pinout-diagrams](https://github.com/lnlp/pinout-diagrams/tree/main/LoRa%20development%20boards).

#### Recommended Hardware
##### Beginners
 [LILYGO® TTGO LORA32](https://www.lilygo.cc/products/lora3?variant=42272562282677)

You get a fully functional board (including antenna) which does not require any additional wiring for a reasonable price!

#### Advanced
##### ESP32 Module
[DFRobot FireBeetle ESP32 IoT](https://www.dfrobot.com/product-1590.html) (DFR0478) recomended due to its good low power design.

##### RFM95W- or SX1276-based Radio Transceiver Module

* [Adafruit RFM95W LoRa Radio Transceiver Breakout](https://www.adafruit.com/product/3072) (ADA3072) - 868/915 MHz version (868 MHz is used for both LoRaWAN and Bresser Weather Sensor in Europe)
  with RF connector (u.FL or SMA as desired)
    * See [Adafruit RFM69HCW and RFM9X LoRa Packet Radio Breakouts - Pinouts](https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/pinouts).

**or**

* [FireBeetle Cover LoRa Radio 868MHz](https://www.dfrobot.com/product-1831.html)

##### Antenna 

* [Delock 89769 - LoRa 868 MHz Antenna SMA plug 3 dBi omnidirectional](https://www.delock.de/produkt/89769/merkmale.html?setLanguage=en) has been used with good results
* [EBYTE TX868-XP-100 868MHz SMA-J 3.5dBi High Gain Omnidirectional Antenna](https://www.amazon.de/gp/product/B07NWLWJH7) - larger, but better

#### Power Supply
Mains adapter or Li-Ion battery (with or without solar charger) - depending on desired operation time and duty cycle.

## Software Build Setup

* Install the Arduino ESP32 board package in the Arduino IDE -<br>
     **Note:** When using the ESP32 board package >=V2.0.5, you have to apply two fixes in arduino-lorawan and arduino-lmic, respectively (see below)
* Select the desired ESP32 board
* Install all libraries as listed in the section [Library Dependencies](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/README.md#library-dependencies) via the Arduino IDE Library Manager
* Configure `Arduino/libraries/MCCI_LoRaWAN_LMIC_library/project_config/lmic_project_config.h`:
   * select you appropriate region
   * `#define CFG_sx1276_radio 1`

* Add the following line to `Arduino/libraries/MCCI_LoRaWAN_LMIC_library/project_config/lmic_project_config.h`:

    `#define LMIC_ENABLE_DeviceTimeReq 1`
    
    (Otherwise requesting the time from the LoRaWAN network will not work, even if supported by the network.)
* Apply fixes if using ESP32 board package >= v2.0.5  
   * https://github.com/mcci-catena/arduino-lorawan/pull/204 (fixed in mcci-catena/arduino-lorawan v0.10.0)
   * https://github.com/mcci-catena/arduino-lmic/issues/714#issuecomment-822051171
* Clone (or download and unpack) the desired BresserWeatherSensorTTN release ([Releases](https://github.com/matthias-bs/BresserWeatherSensorTTN/releases))
* Load the sketch `BresserWeatherSensorTTN.ino` from the BresserWeatherSensorTTN directory
* Compile

### Library Dependencies

| Library                            | r: required /<br>o: optional |
| ---------------------------------- | ---------------------------- |
| MCCI Arduino Development Kit ADK   | r                            |
| MCCI LoRaWAN LMIC library          | r                            |
| MCCI Arduino LoRaWAN Library       | r                            |
| RadioLib                           | r                            |
| LoRa_Serialization                 | r                            |
| ESP32Time                          | r                            |
| BresserWeatherSensorReceiver       | r                            |
| Preferences                        | r (RP2040)                   |
| ESP32AnalogRead                    | o                            |
| OneWireNg                          | o                            |
| DallasTemperature                  | o                            |
| NimBLE-Arduino + ATC_MiThermometer | o                            |
| NimBle-Arduino + Theengs Decoder   | o                            |
| DistanceSensor_A02YYUW             | o                            |

See [package.json](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/package.json) for required/tested versions.

## Software Customization

### Configure the LoRaWAN Network settings APPEUI, DEVEUI and APPKEY

* First you have to follow your LoRaWAN Network provider's instructions on how to configure/obtain the settings.
* Then configure the BresserWeatherSensorTTN software accordingly:
   * Solution 1 (not recommended):
      Configure the section starting with `// APPEUI, DEVEUI and APPKEY` in [BresserWeatherSensorTTN.ino](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/BresserWeatherSensorTTN.ino)
   * Solution 2 (recommended):
   Configure the file [secrets.h](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/secrets.h) - refer to [secrets.h.template](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/secrets.h.template) as an example --
      ```
      #define SECRETS
   
      // deveui, little-endian
      static const std::uint8_t deveui[] = { 0xAA, 0xBB, 0xCC, 0x00, 0x00, 0xDD, 0xEE, 0xFF };
   
      // appeui, little-endian
      static const std::uint8_t appeui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

     // appkey: just a string of bytes, sometimes referred to as "big endian".
     static const std::uint8_t appkey[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00 };
     ```

### Configure the Microcontroller / RF Transceiver GPIO Wiring

#### Pinout Configuration by selecting a supported Board in the Arduino IDE

By selecting a Board and a Board Revision (if available) in the Arduino IDE, a define is passed to the preprocessor/compiler. For the boards in the table below, the default configuration is assumed based in this define.

If you are not using the Arduino IDE, you can use the defines in the table below with your specific tool chain to get the same result.

If this is not what you need, you have to switch to Manual Configuration.

  | Setup                                                          | Board              | Board Revision               | Define                 | Radio Module | Notes    |
   | -------------------------------------------------------------- | ------------------ | ---------------------------- | ---------------------- | -------- | ------- |
   | [LILYGO®TTGO-LORA32 V1](https://github.com/Xinyuan-LilyGo/TTGO-LoRa-Series) | "TTGO LoRa32-OLED" | "TTGO LoRa32 V1 (No TFCard)" | ARDUINO_TTGO_LORA32_V1 | SX1276 (HPD13A) | -   |
   | [LILYGO®TTGO-LORA32 V2](https://github.com/LilyGO/TTGO-LORA32) | "TTGO LoRa32-OLED" | "TTGO LoRa32 V2"             | ARDUINO_TTGO_LoRa32_V2 | SX1276 (HPD13A) | Wire DIO1 to GPIO33 |
   | [LILYGO®TTGO-LORA32 V2.1](https://www.lilygo.cc/products/lora3?variant=42272562282677) | "TTGO LoRa32-OLED" | "TTGO LoRa32 V2.1 (1.6.1)" | ARDUINO_TTGO_LoRa32_v21new |  SX1276 (HPD13A) | - |
   | [Heltec Wireless Stick](https://heltec.org/project/wireless-stick/) | "Heltec Wireless Stick" | n.a. | ARDUINO_heltec_wireless_stick |  SX1276  | - |
   | [LoRaWAN_Node](https://github.com/matthias-bs/LoRaWAN_Node)      | "FireBeetle-ESP32" | n.a.                       | ARDUINO_ESP32_DEV -> LORAWAN_NODE     | SX1276 (RFM95W) | -      |
   | [DFRobot FireBeetle ESP32 IoT Microcontroller](https://www.dfrobot.com/product-1590.html) with [FireBeetle Cover LoRa Radio 868MHz](https://www.dfrobot.com/product-1831.html) | "FireBeetle-ESP32" | n.a.                       | ARDUINO_ESP32_DEV & FIREBEETLE_ESP32_COVER_LORA | SX1276 (LoRa1276) | Wiring on the cover: <br>D2 to RESET<br>D3 to DIO0<br>D4 to CS<br>D5 to DIO1 |
   | [Adafruit Feather ESP32S2 with Adafruit LoRa Radio FeatherWing](https://github.com/matthias-bs/BresserWeatherSensorReceiver#adafruit-feather-esp32s2-with-adafruit-lora-radio-featherwing)                                | "Adafruit Feather ESP32-S2" | n.a.               | ARDUINO_<br>ADAFRUIT_FEATHER_ESP32S2   | SX1276 (RFM95W) | **No Bluetooth available!**<br>Wiring on the Featherwing:<br>E to IRQ<br>D to CS<br>C to RST<br>A to DI01 |
| [Thingpulse ePulse Feather](https://thingpulse.com/product/epulse-feather-low-power-esp32-development-board/) with [Adafruit LoRa Radio FeatherWing](https://www.adafruit.com/product/3231)     | "Adafruit ESP32 Feather" | n.a.               | ARDUINO_FEATHER_ESP32   | SX1276 (RFM95W) | Wiring on the Featherwing:<br>E to IRQ<br>D to CS<br>C to RST<br>A to DI01<br><br>**see** [**#55**](https://github.com/matthias-bs/BresserWeatherSensorTTN/issues/55) |
| [Adafruit Feather RP2040](https://www.adafruit.com/product/4884) with [Adafruit LoRa Radio FeatherWing](https://www.adafruit.com/product/3231)     | "Adafruit Feather RP2040" | n.a.               | ARDUINO_ADAFRUIT_FEATHER_RP2040   | SX1276 (RFM95W) | **No Bluetooth available!**<br>**Configuration: Choose an entry with "FS" in section __Flash Size__!**<br>**Distance sensor not integrated yet!**<br>Wiring on the Featherwing:<br>E to IRQ<br>D to CS<br>C to RST<br>A to DI01 |

If enabled in the Arduino IDE Preferences ("Verbose Output"), the preprosessor will provide some output regarding the selected configuration, e.g.

```
ARDUINO_ADAFRUIT_FEATHER_ESP32S2 defined; assuming RFM95W FeatherWing will be used
[...]
Receiver chip: [SX1276]
Pin config: RST->0 , CS->6 , GD0/G0/IRQ->5 , GDO2/G1/GPIO->11
```

#### Manual Pinout Configuration
**Note:** If you are using the same RF transceiver for sensor data reception and LoRaWAN connection, you must change the pin definitions in **two** places!

1. **LoRaWAN Software Part**

   Change the default configuration in [BresserWeatherSensorTTN.ino](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/BresserWeatherSensorTTN.ino):
   ```
   #define PIN_LMIC_NSS      14
   #define PIN_LMIC_RST      12
   #define PIN_LMIC_DIO0     4
   #define PIN_LMIC_DIO1     16
   #define PIN_LMIC_DIO2     17
   ```

2. **BresserWeatherSensorReceiver Software Part**

   Change the default configuration in the directory **Arduino/libraries/BresserWeatherSensorReceiver/src/WeatherSensorCfg.h**!!! 

   Changing **BresserWeatherSensorTTN/WeatherSensorCfg.h** will have no effect!
   ```
   #define PIN_RECEIVER_CS   14
    
   // CC1101: GDO0 / RFM95W/SX127x: G0
   #define PIN_RECEIVER_IRQ  4 
    
   // CC1101: GDO2 / RFM95W/SX127x: G1
   #define PIN_RECEIVER_GPIO 16
    
   // RFM95W/SX127x - GPIOxx / CC1101 - RADIOLIB_NC
   #define PIN_RECEIVER_RST  12
   ```

### Configure the RF Transceiver SPI Wiring

The board specific default SPI pin definitions (MISO, MOSI and SCK) can be found in 
https://github.com/espressif/arduino-esp32/tree/master/variants

To configure other SPI pins than the default ones... is up to you. I.e. better use the default pins unless you have a really good reason not to do so and then only if you know what you're doing! 

### Other Configuration Options
In [BresserWeatherSensorTTNCfg.h](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/BresserWeatherSensorTTNCfg.h):
* Select the desired LoRaWAN network by (un)-commenting `ARDUINO_LMIC_CFG_NETWORK_TTN` or `ARDUINO_LMIC_CFG_NETWORK_GENERIC`
* Disable features which you do not want to use
* Configure the timing parameters (if you think this is needed) 
* If enabled, configure your ATC MiThermometer's / Theengs Decoder's BLE MAC Address by by editing `KNOWN_BLE_ADDRESSES`
* Configure your time zone by editing `TZ_INFO`
* Configure the ADC's input pins, dividers and oversampling settings as needed

### Change the LoRaWAN Message Payload/Encoding
In [BresserWeatherSensorTTN.ino](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/BresserWeatherSensorTTN.ino), change the code starting with
```
//
// Encode sensor data as byte array for LoRaWAN transmission
//
LoraEncoder encoder(loraData);
```
Make sure that you do not exceed the size of the LoRaWAN uplink payload buffer `loraData[PAYLOAD_SIZE]`. The payload size is limited to 51 bytes by the LMIC library (for a good reason).

If you are using an Integration at the network side (such as an MQTT Integration), make sure you adjust your changes there as well - otherwise decoding the receiving/decoding the messages will fail. 

## Debug Output Configuration

See [Debug Output Configuration in Arduino IDE](DEBUG_OUTPUT.md)

## Remote Configuration via LoRaWAN Downlink

| Command / Response            | Cmd  | Port | Unit    | Data0           | Data1           | Data2           | Data3           |
| ----------------------------- | ---- | ---- | ------- | --------------- | --------------- | --------------- | --------------- |
| CMD_SET_WEATHERSENSOR_TIMEOUT | 0xA0 |      | seconds | timeout[7:0]    |                 |                 |                 |
| CMD_SET_SLEEP_INTERVAL        | 0xA8 |      | seconds | interval[15: 8] | interval[ 7: 0] |                 |                 |
| CMD_SET_SLEEP_INTERVAL_LONG   | 0xA9 |      | seconds | interval[15: 8] | interval[ 7: 0] |                 |                 |
| CMD_RESET_RAINGAUGE           | 0xB0 |      |         | [flags]         |                 |                 |                 |
| CMD_GET_CONFIG                | 0xB1 |         |                 |                 |                 |               |                 |                 |
|    response:               |  |  3       | seconds   | ws_timeout[ 7: 0] | sleep_interval[15: 8] | sleep_interval[ 7: 0] | sleep_interval_long[15: 8] | sleep_interval_long[ 7: 0] |
| CMD_GET_DATETIME              | 0x86 |         |                 |                 |                |                 |
|   response:            |      | 2       | epoch   | unixtime[31:24] | unixtime[23:16] | unixtime[15:8] | unixtime[7:0] |
| CMD_SET_DATETIME              | 0x88 |         |epoch   | unixtime[31:24] | unixtime[23:16] | unixtime[15:8] | unixtime[7:0] |

:warning: Confirmed downlinks should not be used! (see [here](https://www.thethingsnetwork.org/forum/t/how-to-purge-a-scheduled-confirmed-downlink/56849/7) for an explanation.)

### Remote Configuration with The Things Network Console
#### With Payload Formatter

* see [scripts/ttn_downlink_formatter.js](scripts/ttn_downlink_formatter.js) for syntax of commands and responses
* see [The Things Network MQTT Integration and Payload Formatters](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/README.md#the-things-network-mqtt-integration-payload-formatters)

##### Example 1: Set SLEEP_INTERVAL to 360 seconds
1. Build command sequence as JSON string: `{"cmd": "CMD_SET_SLEEP_INTERVAL", "interval": 360}`
2. Send command sequence via The Things Network Console
![TTN Downlink as JSON](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/e61ca412-713b-4972-8c4f-f0068068c323)

##### Example 2: Set Date/Time
1. Get epoch (e.g. from https://www.epochconverter.com) (Example: 1692729833); add an offset (estimated) for time until received (Example: + 64 seconds => 16927298**97**) 
2. Build command sequence as JSON string: {"cmd": "CMD_SET_DATETIME", "epoch": 1692729897} 
3. Send command sequence via The Things Network Console

#### Without Payload Formatter
##### Example 1: Set SLEEP_INTERVAL to 360 seconds
1. Convert interval to hex: 360 = 0x0168
2. Build command sequence: "CMD_SET_SLEEP_INTERVAL 360 secs" -> 0xA8 0x01 0x68
3. Send command sequence via The Things Network Console
![TTN Downlink as Hex](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/67544195-c2cd-4118-8de5-1f1c0facacdb)

##### Example 2: Set Date/Time
1. Get epoch (e.g. from https://www.epochconverter.com/hex) (Example: 0x63B2BC32); add an offset (estimated) for time until received (Example: + 64 / 0x40 seconds => 0x63B2BC**7**2) 
2. Build command sequence: "CMD_SET_DATETIME 0x63B2BC72" -> 0x88 0x63 0xB2 0xBC 0x72
3. Send command sequence via The Things Network Console

### Remote Configuration with Helium Console
#### With Payload Formatter
_To be done_

#### Without Payload Formatter 
##### Example 1: Set SLEEP_INTERVAL to 360 seconds
1. Convert interval to hex: 360 = 0x0168
2. Build command sequence: "CMD_SET_SLEEP_INTERVAL 360 secs" -> 0xA8 0x01 0x68
3. Convert command sequence to Base64 encoding: 0xA8 0x01 0x68 -> "qAFo" ([Base64 Guru](https://base64.guru/converter/encode/hex))
4. Send command sequence via Helium Console
    ![Helium_Add_Downlink_Payload](https://user-images.githubusercontent.com/83612361/210183244-a2d109bc-6782-4f83-b406-7f6e0b17eda1.png)

##### Example 2: Set Date/Time

1. Get epoch (e.g. from https://www.epochconverter.com/hex) (Example: 0x63B2BC32); add an offset (estimated) for time until received (Example: + 64 / 0x40 seconds => 0x63B2BC**7**2) 
2. Build command sequence: "CMD_SET_DATETIME 0x63B2BC72" -> 0x88 0x63 0xB2 0xBC 0x72
3. Convert command sequence to Base64 encoding: 0x88 0x63 0xB2 0xBC 0x72 -> "iGOyvHI="
4. Send command sequence e.g. via Helium Console


## MQTT Integration

### The Things Network MQTT Integration and Payload Formatters

#### Uplink Formatter

Decode uplink payload (a sequence of bytes) into data structures which are readable/suitable for further processing.

In The Things Network Console:
1. Go to "Payload formatters" -> "Uplink"
2. Select "Formatter type": "Custom Javascript formatter"
3. "Formatter code": Paste [scripts/ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js)
4. Apply "Save changes"

![TTN Uplink Formatter](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/38b66478-688a-4028-974a-c517cddae662)

#### Downlink Formatter

Encode downlink payload from JSON to a sequence of bytes.

In The Things Network Console:
1. Go to "Payload formatters" -> "Downlink"
2. Select "Formatter type": "Custom Javascript formatter"
3. "Formatter code": Paste [scripts/ttn_downlink_formatter.js](scripts/ttn_downlink_formatter.js)
4. Apply "Save changes"

**Note:** The actual payload depends on the options selected in the Arduino software - the decoder must be edited accordingly (add or remove data types and JSON identifiers - see [scripts/ttn_uplink_formatter.js line 316ff](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/c64593e113f11b120eea79c6558f4f0822c4af91/scripts/ttn_uplink_formatter.js#L316)). The configuration dependent part of the decoder can be created with a C++ preprocessor and the Python script [generate_decoder.py](https://github.com/matthias-bs/BresserWeatherSensorTTN/blob/main/scripts/generate_decoder.py).

#### MQTT Integration
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


JSON-Path with Uplink-Decoder (see [scripts/ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js))

`.uplink_message.decoded_payload.bytes.<variable>`

### Helium Network MQTT Integration and Message Decoder

Please refer to https://docs.helium.com/use-the-network/console/integrations/mqtt/.

Add an MQTT integration in the Helium console - the "Endpoint" is in fact an MQTT broker you have to provide:
![Helium_MQTT_Integration](https://user-images.githubusercontent.com/83612361/195050719-8562ad0e-5523-436f-8b61-e4b15b08d6de.png)

Add [scripts/helium_decoder.js](scripts/helium_decoder.js) in the Helium console as custom function:
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
### Datacake Integration

#### Desktop Dashboard

![Datacake_Dashboard_Desktop](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/2a876ba1-06b9-4ea3-876c-2fad3d559b01)

#### Mobile Dashboard
![Datacake_Dashboard_Mobile](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/fbc0948c-bfd8-4d7d-9780-c113d576d3cf)

#### Datacake/TTN Setup

YouTube Video: [Get started for free with LoRaWaN on The Things Network and Datacake IoT Platform](https://youtu.be/WGVFgYp3k3s)

[Datacake Payload Decoder](scripts/datacake_decoder.js)

### Decoder Scripts Summary

#### [decoder_basic.js](scripts/decoder_basic.js)
    
Basic example for a single Weather Sensor uplink to [The Things Network](https://www.thethingsnetwork.org/).

#### [ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js)
    
[The Things Network](https://www.thethingsnetwork.org/) uplink formatter for the full set of features provided by the default SW configuration. Includes decoding of response messages triggered by uplink command messages.

#### [ttn_downlink_formatter.js](scripts/ttn_downlink_formatter.js)

[The Things Network](https://www.thethingsnetwork.org/) downlink formatter for sending commands as JSON strings to the device.

#### [ttn_decoder_distance.js](scripts/ttn_decoder_distance.js)

Variant of [ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js) which supports the ultrasonic distance sensor, but neither the lightning sensor nor response messages.

#### [ttn_decoder_fp.js](scripts/ttn_decoder_fp.js)
    
Predecessor of [ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js) without support of response messages. *deprecated, will eventually be deleted*

#### [helium_decoder.js](scripts/helium_decoder.js)
    
Variant of [ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js) for [Helium Network](https://www.helium.com/), currently without support of response messages.

#### [datacake_decoder.js](scripts/datacake_decoder.js)
    
Equivalent of [ttn_uplink_formatter.js](scripts/ttn_uplink_formatter.js) for [Datacake](https://datacake.co/), currently without support of response messages.

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
* [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) by h2zero
* [Theengs Decoder](https://github.com/theengs/decoder) by [Theengs Project](https://github.com/theengs)
* [DistanceSensor_A02YYUW](https://github.com/pportelaf/DistanceSensor_A02YYUW) by Pablo Portela
* [Preferences](https://github.com/vshymanskyy/Preferences) by Volodymyr Shymanskyy

# Legal

> This project is in no way affiliated with, authorized, maintained, sponsored or endorsed by Bresser GmbH or any of its affiliates or subsidiaries.
