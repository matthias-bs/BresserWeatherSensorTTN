///////////////////////////////////////////////////////////////////////////////
// BresserWeatherSensorTTNCfg.h
// 
// User specific configuration for BresserWeatherSensorTTN.ino
//
// - Enabling or disabling of features
// - Timing configuration
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
//
// ToDo:
// - 
//
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

// Enable debug mode (debug messages via serial port)
#define _BWS_DEBUG_MODE_

// Enable TTN debug mode - this generates dummy weather data and skips weather sensor reception 
//#define TTN_DEBUG

// Enable sleep mode - sleep after successful transmission to TTN (recommended!)
#define SLEEP_EN

// If SLEEP_EN is defined, MCU will sleep for SLEEP_INTERVAL seconds after succesful transmission
#define SLEEP_INTERVAL 360

// RTC to network time sync interval (in minutes)
//#define CLOCK_SYNC_INTERVAL 24 * 60
#define CLOCK_SYNC_INTERVAL 15

// Force deep sleep after a certain time, even if transmission was not completed
#define FORCE_SLEEP

// During initialization (not joined), force deep sleep after SLEEP_TIMEOUT_INITIAL (if enabled)
#define SLEEP_TIMEOUT_INITIAL 1800

// If already joined, force deep sleep after SLEEP_TIMEOUT_JOINED seconds (if enabled)
#define SLEEP_TIMEOUT_JOINED 600

// Additional timeout to be applied after joining if Network Time Request pending
#define SLEEP_TIMEOUT_EXTRA 300

// Timeout for weather sensor data reception (seconds)
#define WEATHERSENSOR_TIMEOUT 120

// Enable transmission of weather sensor ID
//#define SENSORID_EN

// Enable battery / supply voltage measurement
#define ADC_EN

// Enable OneWire temperature measurement
#define ONEWIRE_EN

// Enable BLE temperature/humidity measurement
// Note: BLE requires a lot of program memory!
#define MITHERMOMETER_EN

// Enable Bresser Soil Temperature/Moisture Sensor
#define SOILSENSOR_EN

// ADC for supply/battery voltage measurement
// default: on-board connection to VB on FireBeetle ESP32 (with R10+R11 assembled)
#ifdef ADC_EN
    #define PIN_ADC_IN        A0
    //#define PIN_ADC_IN        34
#endif

// Additional ADC pins (default: FireBeetle ESP32) 
//#define PIN_ADC0_IN         A0
//#define PIN_ADC1_IN         A1
//#define PIN_ADC2_IN         A2
#define PIN_ADC3_IN         A3

#ifdef PIN_ADC0_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC3_DIV         = 0.5;       
    const uint8_t ADC3_SAMPLES   = 10;
#endif

#ifdef PIN_ADC1_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC3_DIV         = 0.5;       
    const uint8_t ADC3_SAMPLES   = 10;
#endif

#ifdef PIN_ADC2_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC3_DIV         = 0.5;       
    const uint8_t ADC3_SAMPLES   = 10;
#endif

#ifdef PIN_ADC3_IN
    // Voltage divider R1 / (R1 + R2) -> V_meas = V(R1 + R2); V_adc = V(R1)
    const float ADC3_DIV         = 0.5;       
    const uint8_t ADC3_SAMPLES   = 10;
#endif

#ifdef ONEWIRE_EN
    #define PIN_ONEWIRE_BUS   5
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

// Central European Summer Time - begins last Sunday in March   at 1:00 UTC
TimeChangeRule euCEST = {"CEST", Last, Sun, Mar, 1, 120};  //UTC + 2 hours
// Central European Time        - begins last Sunday in October at 1:00 UTC
TimeChangeRule euCET  = {"EST",  Last, Sun, Oct, 1,  60};  //UTC + 1 hours
Timezone euCentral(euCEST, euCET);
