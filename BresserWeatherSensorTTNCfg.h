///////////////////////////////////////////////////////////////////////////////
// BresserWeatherSensorTTNCfg.h
// 
// User specific configuration for BresserWeatherSensorTTN.ino
//
// - Enabling or disabling of features
// - Voltage thresholds for power saving
// - Timing configuration
// - Timezone
//
// created: 08/2022
//
//
// MIT License
//
// Copyright (c) 2022 Matthias Prinke
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// History:
//
// 20220819 Created from BresserWeatherSensorTTN.ino
// 20221011 Changed timezone handling
// 20221113 Fixed ADC defines
// 20221117 Enabled FORCE_JOIN_AFTER_SLEEP_TIMEOUT per default
//          Added defines for power saving - 
//              BATTERY_WEAK, BATTERY_LOW, SLEEP_INTERVAL_LONG
// 20221228 Modified DEBUG_PRINTF/DEBUG_PRINTF_TS macros to use
//          Arduino logging functions
// 20221230 Added WEATHERSENSOR_DATA_REQUIRED
// 20220112 Removed LMIC_ENABLE_DeviceTimeReq; must be defined in 
//          ~/Arduino/libraries/MCCI_LoRaWAN_LMIC_library/project_config/
//          lmic_project_config.h
//          in order to be recognized!!!
// 20230121 Added configuration for TTGO LoRa32 V1
// 20230209 Added configurations for TTGO LoRa32 V2/V21new,
//          Adafruit Feather ESP32-S2 and Adafruit Feather ESP32
//
// ToDo:
// - 
//
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

// Enable debug mode (debug messages via serial port)
// Arduino IDE: Tools->Core Debug Level: "Debug|Verbose"
//#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_DEBUG
//#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_VERBOSE

//--- Select Board ---
#if !defined(ARDUINO_TTGO_LoRa32_V1)     && !defined(ARDUINO_TTGO_LoRa32_V2) && \
    !defined(ARDUINO_TTGO_LoRa32_v21new) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && \
    !defined(ARDUINO_FEATHER_ESP32)
    // Use pinning for LoRaWAN Node 
    #define LORAWAN_NODE
#endif

//--- Select LoRaWAN Network ---
// The Things Network
#define ARDUINO_LMIC_CFG_NETWORK_TTN 1

// Helium Network
// see mcci-cathena/arduino-lorawan issue #185 "Add Helium EU868 support"
// (https://github.com/mcci-catena/arduino-lorawan/issues/185)
#define ARDUINO_LMIC_CFG_NETWORK_GENERIC 0

// Enable LORAWAN debug mode - this generates dummy weather data and skips weather sensor reception 
//#define LORAWAN_DEBUG

// Battery voltage thresholds for energy saving

// If SLEEP_EN is defined and battery voltage is below BATTERY_WEAK [mV], MCU will sleep for SLEEP_INTERVAL_LONG
#define BATTERY_WEAK 3500

// Go to sleep mode immediately after start if battery voltage is below BATTERY_LOW [mV]
#define BATTERY_LOW 3200

// Enable sleep mode - sleep after successful transmission to TTN (recommended!)
#define SLEEP_EN

// If SLEEP_EN is defined, MCU will sleep for SLEEP_INTERVAL seconds after succesful transmission
#define SLEEP_INTERVAL 360

// Long sleep interval, MCU will sleep for SLEEP_INTERVAL_LONG seconds if battery voltage is below BATTERY_WEAK
#define SLEEP_INTERVAL_LONG 900

// RTC to network time sync interval (in minutes)
#define CLOCK_SYNC_INTERVAL 24 * 60

// Force deep sleep after a certain time, even if transmission was not completed
#define FORCE_SLEEP

// Force a new join procedure (instead of re-join) after encountering sleep timeout
#define FORCE_JOIN_AFTER_SLEEP_TIMEOUT

// During initialization (not joined), force deep sleep after SLEEP_TIMEOUT_INITIAL (if enabled)
#define SLEEP_TIMEOUT_INITIAL 1800

// If already joined, force deep sleep after SLEEP_TIMEOUT_JOINED seconds (if enabled)
#define SLEEP_TIMEOUT_JOINED 600

// Additional timeout to be applied after joining if Network Time Request pending
#define SLEEP_TIMEOUT_EXTRA 300

// Timeout for weather sensor data reception (seconds)
#define WEATHERSENSOR_TIMEOUT 180

// If enabled, enter deep sleep mode if receiving weather sensor data was not successful
//#define WEATHERSENSOR_DATA_REQUIRED

// Enable transmission of weather sensor ID
//#define SENSORID_EN

// Enable rain data statistics
#define RAINDATA_EN

// Enable battery / supply voltage measurement
#define ADC_EN

// Enable OneWire temperature measurement
#define ONEWIRE_EN

// Enable BLE temperature/humidity measurement
// Notes: 
// * BLE requires a lot of program memory!
// * ESP32-S2 does not provide BLE!
#ifndef ARDUINO_ADAFRUIT_FEATHER_ESP32S2
    #define MITHERMOMETER_EN
#endif

// Enable Bresser Soil Temperature/Moisture Sensor
#define SOILSENSOR_EN

// ADC for supply/battery voltage measurement
// default: on-board connection to VB on FireBeetle ESP32 (with R10+R11 assembled)
//          on-board connection to VBAT on TTGO LoRa32
//          on-board connection to VBAT on Adafruit Feather ESP32
//          no VBAT input circuit on Adafruit Feather ESP32-S2
#ifdef ADC_EN
    #if defined(ARDUINO_TTGO_LoRa32_V1) || defined(ARDUINO_TTGO_LoRa32_V2) || defined(ARDUINO_TTGO_LoRa32_v21new)
        #define PIN_ADC_IN        35
    #elif defined(ARDUINO_FEATHER_ESP32)
        #define PIN_ADC_IN        A13
    #elif defined(LORAWAN_NODE)
        #define PIN_ADC_IN        A0
    #else
        #define PIN_ADC_IN        34
    #endif
#endif


// Additional ADC pins (default: FireBeetle ESP32) 
//#define PIN_ADC0_IN         A0
//#define PIN_ADC1_IN         A1
//#define PIN_ADC2_IN         A2
#ifdef LORAWAN_NODE
  #define PIN_ADC3_IN         A3
#endif

#ifdef PIN_ADC0_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC0_DIV         = 0.5;       
    const uint8_t ADC0_SAMPLES   = 10;
#endif

#ifdef PIN_ADC1_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC1_DIV         = 0.5;       
    const uint8_t ADC1_SAMPLES   = 10;
#endif

#ifdef PIN_ADC2_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC2_DIV         = 0.5;       
    const uint8_t ADC2_SAMPLES   = 10;
#endif

#ifdef PIN_ADC3_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC3_DIV         = 0.5;       
    const uint8_t ADC3_SAMPLES   = 10;
#endif

#ifdef ONEWIRE_EN
    #if defined(ARDUINO_TTGO_LoRa32_V1)
        #define PIN_ONEWIRE_BUS   21
    #elif defined(ARDUINO_FEATHER_ESP32)
        #define PIN_ONEWIRE_BUS   15
    #elif defined(LORAWAN_NODE)
        #define PIN_ONEWIRE_BUS   5
    #else
        #define PIN_ONEWIRE_BUS   0
    #endif
#endif

#ifdef ADC_EN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float UBATT_DIV         = 0.5;       
    const uint8_t UBATT_SAMPLES   = 10;
#endif

#ifdef MITHERMOMETER_EN
    // BLE scan time in seconds
    const int bleScanTime = 10;

    // List of known sensors' BLE addresses
    std::vector<std::string> knownBLEAddresses = {"a4:c1:38:b8:1f:7f"};
#endif

// Enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)
const char* TZ_INFO    = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  
