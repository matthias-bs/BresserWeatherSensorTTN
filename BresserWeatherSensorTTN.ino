///////////////////////////////////////////////////////////////////////////////
// BresserWeatherSensorTTN.ino
// 
// Bresser 5-in-1/6-in-1 868 MHz Weather Sensor Radio Receiver 
// based on ESP32 and RFM95W - 
// sends data to a LoRaWAN network (e.g. The Things Network)
//
// The RFM95W radio transceiver is used
// in FSK mode to receive weather sensor data
// and
// in LoRaWAN mode to connect to a LoRaWAN network
//
// Based on:
// ---------
// Bresser5in1-CC1101 by Sean Siford (https://github.com/seaniefs/Bresser5in1-CC1101)
// https://github.com/merbanan/rtl_433/blob/master/src/devices/bresser_6in1.c
// RadioLib by Jan Gromeš (https://github.com/jgromes/RadioLib)
// MCCI LoRaWAN LMIC library by Thomas Telkamp and Matthijs Kooijman / Terry Moore, MCCI (https://github.com/mcci-catena/arduino-lmic)
// MCCI Arduino LoRaWAN Library by Terry Moore, MCCI (https://github.com/mcci-catena/arduino-lorawan)
// Lora-Serialization by Joscha Feth (https://github.com/thesolarnomad/lora-serialization)
// ESP32Time by Felix Biego (https://github.com/fbiego/ESP32Time)
// ESP32AnalogRead by Kevin Harrington (madhephaestus) (https://github.com/madhephaestus/ESP32AnalogRead)
// OneWireNg by Piotr Stolarz (https://github.com/pstolarz/OneWireNg)
// DallasTemperature / Arduino-Temperature-Control-Library by Miles Burton (https://github.com/milesburton/Arduino-Temperature-Control-Library) 
//
// Library dependencies (tested versions):
// ---------------------------------------
// (install via normal Arduino Library installer:) 
// MCCI Arduino Development Kit ADK     0.2.2
// MCCI LoRaWAN LMIC library            4.1.1
// MCCI Arduino LoRaWAN Library         0.9.2
// RadioLib                             6.1.0
// LoRa_Serialization                   3.2.1
// ESP32Time                            2.0.3
// BresserWeatherSensorReceiver         0.12.1
// ESP32AnalogRead                      0.2.1 (optional)
// OneWireNg                            0.13.1 (optional)
// DallasTemperature                    3.9.0 (optional)
// NimBLE-Arduino                       1.4.1 (optional)
// ATC MiThermometer                    0.2.1 (optional)
// Theengs Decoder                      1.5.7 (optional)
//
// (installed from ZIP file:)
// DistanceSensor_A02YYUW               1.0.2 (optional)
//
// created: 06/2022
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
// 20220620 Created
// 20220816 Added support of multiple Bresser 868 MHz sensors; 
//          e.g. weather sensor and soil temperature/moisture sensor
// 20220825 Added time keeping with RTC and synchronization to network time
// 20220915 Added monthly/daily/hourly precipitation values
// 20221010 Changed cMyLoRaWAN to inherit from Arduino_LoRaWAN_network
//          instead of Arduino_LoRaWAN_ttn
//          Moved LoRaWAN network selection to BresserWeatherSensorTTNCfg.h
//          Changed LoRaWAN message size to actual payload size
// 20221011 Replaced Timezone library by ESP32's internal TZ handling 
// 20221109 Updated BresserWeatherSensorReceiver to v0.4.0
// 20221117 Implemented wake-up to fixed time scheme
//          Added energy saving modes
// 20221228 Modified DEBUG_PRINTF/DEBUG_PRINTF_TS macros to use
//          Arduino logging functions
// 20221230 Added compile time option to enter deep sleep mode
//          if receiving weather sensor data was not successful
// 20221231 Added setting of RTC via downlink
// 20230101 Added remote configuration via LoRaWAN downlink
// 20230112 Fixed rain gauge update in case RTC was set by LoRaWAN downlink
//          Added note regarding LMIC_ENABLE_DeviceTimeReq
// 20230121 Added configuration for TTGO LoRa32 V1
// 20230208 Added configurations for TTGO LoRa32 V2 and V2.1
// 20230209 Added configuration for Adafruit Feather ESP32-S2 with RFM95W FeatherWing
//          Added configuration for Adafruit Huzzah ESP32 Feather with RFM95W FeatherWing
// 20230211 Added integration of Theengs Decoder (https://github.com/theengs/decoder)
//          for support of additional BLE sensors
// 20230217 Added integration of A02YYUW (DFRobot SEN0311) 
//          ultrasonic distance sensor 
//          (https://wiki.dfrobot.com/_A02YYUW_Waterproof_Ultrasonic_Sensor_SKU_SEN0311)
// 20230304 Added configuration for Heltec Wireless Stick
// 20230614 Added configuration for Heltec WiFi LoRa 32
// 20230705 Updated library versions
// 20230714 Added integration of Bresser Lightning Sensor
//
// ToDo:
// -  
//
// Notes:
// - Pin mapping of radio transceiver module is done in two places:
//   - MCCI Arduino LoRaWAN Library:         this file (see below)
//   - BresserWeatherSensorReceiver Library: WeatherSensorCfg.h
// - After a successful transmission, the controller can go into deep sleep
// - If joining the network or transmitting uplink data fails,
//   the controller can go into deep sleep
// - Weather sensor data is only received at startup; this avoids
//   reconfiguration of the RFM95W module and its SW drivers - 
//   i.e. to work as a weather data relay to TTN, enabling sleep mode 
//   is basically the only useful option
// - The ESP32's RTC RAM is used to store information about the LoRaWAN 
//   network session; this speeds up the connection after a restart
//   significantly
// - The ESP32's Bluetooth LE interface is used to access sensor data (option)
// - To enable Network Time Requests:
//   #define LMIC_ENABLE_DeviceTimeReq 1
// - settimeofday()/gettimeofday() must be used to access the ESP32's RTC time
// - Arduino ESP32 package has built-in time zone handling, see 
//   https://github.com/SensorsIot/NTP-time-for-ESP8266-and-ESP32/blob/master/NTP_Example/NTP_Example.ino
// - Apply fixes if using Arduino ESP32 board package v2.0.x
//     - mcci-catena/arduino-lorawan#204
//       (https://github.com/mcci-catena/arduino-lorawan/pull/204)
//     - mcci-catena/arduino-lmic#714 
//       (https://github.com/mcci-catena/arduino-lmic/issues/714#issuecomment-822051171)
//
///////////////////////////////////////////////////////////////////////////////
/*! \file */ 

#include "BresserWeatherSensorTTNCfg.h"
#include <Arduino_LoRaWAN_network.h>
#include <Arduino_LoRaWAN_EventLog.h>
#include <arduino_lmic.h>
#include <Preferences.h>
#include <ESP32Time.h>


#ifdef RAINDATA_EN
    #include "RainGauge.h"
#endif

// NOTE: Add #define LMIC_ENABLE_DeviceTimeReq 1
//        in ~/Arduino/libraries/MCCI_LoRaWAN_LMIC_library/project_config/lmic_project_config.h
#if (not(LMIC_ENABLE_DeviceTimeReq))
    #warning "LMIC_ENABLE_DeviceTimeReq is not set - will not be able to retrieve network time!"
#endif

#ifndef SLEEP_EN
    #warning "SLEEP_EN is not defined, but the weather sensors are only read once during (re-)start! You have been warned!"
#endif

#ifdef ONEWIRE_EN
    // Dallas/Maxim OneWire Temperature Sensor
    #include <DallasTemperature.h>
#endif

#ifdef DISTANCESENSOR_EN
    // A02YYUW / DFRobot SEN0311 Ultrasonic Distance Sensor
    #include <DistanceSensor_A02YYUW.h>
#endif

#ifdef ADC_EN
    // ESP32 calibrated Analog Input Reading
    #include <ESP32AnalogRead.h>
#endif

// BresserWeatherSensorReceiver
#include "WeatherSensorCfg.h"
#include "WeatherSensor.h"

#if defined(MITHERMOMETER_EN)
    // BLE Temperature/Humidity Sensor
    #include <ATC_MiThermometer.h>
#endif
#if defined(THEENGSDECODER_EN)
    #include "src/BleSensors/BleSensors.h"
#endif

// LoRa_Serialization
#include <LoraMessage.h>

// Pin mapping for ESP32 (MCCI Arduino LoRaWAN Library)
// Note: Pin mapping for BresserWeatherSensorReceiver is done in WeatherSensorCfg.h!
// SPI2 is used on ESP32 per default! (e.g. see https://github.com/espressif/arduino-esp32/tree/master/variants/doitESP32devkitV1)
#if defined(ARDUINO_TTGO_LoRa32_V1)
    // https://github.com/espressif/arduino-esp32/blob/master/variants/ttgo-lora32-v1/pins_arduino.h
    // http://www.lilygo.cn/prod_view.aspx?TypeId=50003&Id=1130&FId=t3:50003:3
    // https://github.com/Xinyuan-LilyGo/TTGO-LoRa-Series
    // https://github.com/LilyGO/TTGO-LORA32/blob/master/schematic1in6.pdf
    #define PIN_LMIC_NSS      LORA_CS
    #define PIN_LMIC_RST      LORA_RST
    #define PIN_LMIC_DIO0     LORA_IRQ
    #define PIN_LMIC_DIO1     33
    #define PIN_LMIC_DIO2     cMyLoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN

#elif defined(ARDUINO_TTGO_LoRa32_V2)
    // https://github.com/espressif/arduino-esp32/blob/master/variants/ttgo-lora32-v2/pins_arduino.h
    #define PIN_LMIC_NSS      LORA_CS
    #define PIN_LMIC_RST      LORA_RST
    #define PIN_LMIC_DIO0     LORA_IRQ
    #define PIN_LMIC_DIO1     33
    #define PIN_LMIC_DIO2     cMyLoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN
    #pragma message("LoRa DIO1 must be wired to GPIO33 manually!")

#elif defined(ARDUINO_TTGO_LoRa32_v21new)
    // https://github.com/espressif/arduino-esp32/blob/master/variants/ttgo-lora32-v21new/pins_arduino.h
    #define PIN_LMIC_NSS      LORA_CS
    #define PIN_LMIC_RST      LORA_RST
    #define PIN_LMIC_DIO0     LORA_IRQ
    #define PIN_LMIC_DIO1     LORA_D1
    #define PIN_LMIC_DIO2     LORA_D2

#elif defined(ARDUINO_heltec_wireless_stick) || defined(ARDUINO_heltec_wifi_lora_32_V2)
    // https://github.com/espressif/arduino-esp32/blob/master/variants/heltec_wireless_stick/pins_arduino.h
    // https://github.com/espressif/arduino-esp32/tree/master/variants/heltec_wifi_lora_32_V2/pins_ardiono.h
    #define PIN_LMIC_NSS      SS
    #define PIN_LMIC_RST      RST_LoRa
    #define PIN_LMIC_DIO0     DIO0
    #define PIN_LMIC_DIO1     DIO1
    #define PIN_LMIC_DIO2     DIO2

#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
    #define PIN_LMIC_NSS      6
    #define PIN_LMIC_RST      9
    #define PIN_LMIC_DIO0     5
    #define PIN_LMIC_DIO1     11
    #define PIN_LMIC_DIO2     cMyLoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN
    #pragma message("ARDUINO_ADAFRUIT_FEATHER_ESP32S2 defined; assuming RFM95W FeatherWing will be used")
    #pragma message("Required wiring: E to IRQ, D to CS, C to RST, A to DI01")
    #pragma message("BLE is not available!")

#elif defined(ARDUINO_FEATHER_ESP32)
    #define PIN_LMIC_NSS      14
    #define PIN_LMIC_RST      27
    #define PIN_LMIC_DIO0     32
    #define PIN_LMIC_DIO1     33
    #define PIN_LMIC_DIO2     cMyLoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN
    #pragma message("NOT TESTED!!!")
    #pragma message("ARDUINO_ADAFRUIT_FEATHER_ESP32 defined; assuming RFM95W FeatherWing will be used")
    #pragma message("Required wiring: A to RST, B to DIO1, D to DIO0, E to CS")

#else
    // LoRaWAN_Node board
    // https://github.com/matthias-bs/LoRaWAN_Node
    #define PIN_LMIC_NSS      14
    #define PIN_LMIC_RST      12
    #define PIN_LMIC_DIO0     4
    #define PIN_LMIC_DIO1     16
    #define PIN_LMIC_DIO2     17

#endif


// Uplink message payload size
// The maximum allowed for all data rates is 51 bytes.
const uint8_t PAYLOAD_SIZE = 51;

// RTC Memory Handling
#define MAGIC1 (('m' << 24) | ('g' < 16) | ('c' << 8) | '1')
#define MAGIC2 (('m' << 24) | ('g' < 16) | ('c' << 8) | '2')
#define EXTRA_INFO_MEM_SIZE 64

// To enable debug mode (debug messages via serial port):
// Arduino IDE: Tools->Core Debug Level: "Debug|Verbose"
// or
// set CORE_DEBUG_LEVEL in BresserWeatherSensorTTNCfg.h
#define DEBUG_PRINTF(...) { log_d(__VA_ARGS__); }
#define DEBUG_PRINTF_TS(...) { log_d(__VA_ARGS__); }

// Downlink messages
//
// CMD_SET_WEATHERSENSOR_TIMEOUT
// (seconds)
// byte0: 0xA0
// byte1: <timeout_in_secs>
//
// CMD_SET_SLEEP_INTERVAL
// (seconds)
// byte0: 0xA8
// byte1: sleep_interval[15:8]
// byte2: sleep_interval[ 7:0]

// CMD_SET_SLEEP_INTERVAL_LONG
// (seconds)
// byte0: 0xA9
// byte1: sleep_interval_long[15:8]
// byte2: sleep_interval_long[ 7:0]
//
// CMD_RESET_RAINGAUGE
// byte0: 0xB0
//
// CMD_SET_DATETIME
// byte0: 0x88
// byte1: unixtime[31:24]
// byte2: unixtime[23:16]
// byte3: unixtime[15: 8]
// byte4: unixtime[ 7: 0]

#define CMD_SET_WEATHERSENSOR_TIMEOUT   0xA0
#define CMD_SET_SLEEP_INTERVAL          0xA8
#define CMD_SET_SLEEP_INTERVAL_LONG     0xA9
#define CMD_RESET_RAINGAUGE             0xB0
#define CMD_SET_DATETIME                0x88

void printDateTime(void);
    
/****************************************************************************\
|
|	The LoRaWAN object
|
\****************************************************************************/

/*!
 * \class cMyLoRaWAN
 * 
 * \brief The LoRaWAN object - LoRaWAN protocol and session handling
 */ 
class cMyLoRaWAN : public Arduino_LoRaWAN_network {
public:
    cMyLoRaWAN() {};
    using Super = Arduino_LoRaWAN_network;
    
    /*!
     * \fn setup
     * 
     * \brief Initialize cMyLoRaWAN object
     */
    void setup();
    
    
    /*!
     * \fn requestNetworkTime
     * 
     * \brief Wrapper function for LMIC_requestNetworkTime()
     */
    void requestNetworkTime(void);
    
    
    /*!
     * \fn printSessionInfo
     * 
     * \brief Print contents of session info data structure for debugging
     * 
     * \param Info Session information data structure
     */
    void printSessionInfo(const SessionInfo &Info);
    
    
    /*!
     * \fn printSessionState
     * 
     * \brief Print contents of session state data structure for debugging
     * 
     * \param State Session state data structure
     */
    void printSessionState(const SessionState &State);
    
protected:
    // you'll need to provide implementation for this.
    virtual bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo*) override;

    // NetTxComplete() activates deep sleep mode (if enabled)
    virtual void NetTxComplete(void) override;

    // NetJoin() changes <sleepTimeout>
    virtual void NetJoin(void) override;
    
    // Used to store/load data to/from persistent (at least during deep sleep) memory 
    virtual void NetSaveSessionInfo(const SessionInfo &Info, const uint8_t *pExtraInfo, size_t nExtraInfo) override;
    //virtual bool GetSavedSessionInfo(SessionInfo &Info, uint8_t*, size_t, size_t*) override;
    virtual void NetSaveSessionState(const SessionState &State) override;
    virtual bool NetGetSessionState(SessionState &State) override;
    virtual bool GetAbpProvisioningInfo(AbpProvisioningInfo *pAbpInfo) override;  
};


/****************************************************************************\
|
|	The sensor object
|
\****************************************************************************/

/*!
 * \class cSensor
 * 
 * \brief The sensor object - collect sensor data and schedule uplink
 */
class cSensor {
public:
    /// \brief the constructor. Deliberately does very little.
    cSensor() {};

    /*!
     * \fn getTemperature
     * 
     * \brief Get temperature from DS18B20 sensor via OneWire bus
     * 
     * \returns Temperature [degC]
     */
    float getTemperature(void);
    
    #ifdef ADC_EN
        /*!
         * \fn getVoltage
         * 
         * \brief Get supply voltage (fixed ADC input circuit on FireBeetle ESP32 board)
         * 
         * \returns Voltage [mV]
         */
        uint16_t getVoltage(void);
        
        /*
         * \fn getVoltage
         * 
         * \brief Get ADC voltage from specified port with averaging and application of divider
         * 
         * \param adc ADC port
         * 
         * \param samples No. of samples used in averaging
         * 
         * \param divider Voltage divider
         * 
         * \returns Voltage [mV]
         */
        uint16_t getVoltage(ESP32AnalogRead &adc, uint8_t samples, float divider);
    #endif
        
    /*!
     * \fn uplinkRequest
     * 
     * \brief Request uplink to LoRaWAN
     */
    void uplinkRequest(void) {
        m_fUplinkRequest = true;
    };
    
    /*!
     * \brief set up the sensor object
     *
     * \param uplinkPeriodMs optional uplink interval. If not specified,
     *                       transmit every six minutes.
     */
    void setup(std::uint32_t uplinkPeriodMs = 6 * 60 * 1000);

    /*!
     * \brief update sensor loop.
     *
     * \details
     *     This should be called from the global loop(); it periodically
     *     gathers and transmits sensor data.
     */
    void loop();

    
private:
    void doUplink();

    bool m_fUplinkRequest;              // set true when uplink is requested
    bool m_fBusy;                       // set true while sending an uplink
    std::uint32_t m_uplinkPeriodMs;     // uplink period in milliseconds
    std::uint32_t m_tReference;         // time of last uplink
};

/****************************************************************************\
|
|	Globals
|
\****************************************************************************/

// the global LoRaWAN instance.
cMyLoRaWAN myLoRaWAN {};

// the global sensor instance
cSensor mySensor {};

// the global event log instance
Arduino_LoRaWAN::cEventLog myEventLog;

// The pin map. This form is convenient if the LMIC library
// doesn't support your board and you don't want to add the
// configuration to the library (perhaps you're just testing).
// This pinmap matches the FeatherM0 LoRa. See the arduino-lmic
// docs for more info on how to set this up.
const cMyLoRaWAN::lmic_pinmap myPinMap = {
     .nss = PIN_LMIC_NSS,
     .rxtx = cMyLoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN,
     .rst = PIN_LMIC_RST,
     .dio = { PIN_LMIC_DIO0, PIN_LMIC_DIO1, PIN_LMIC_DIO2 },
     .rxtx_rx_active = 0,
     .rssi_cal = 0,
     .spi_freq = 8000000,
     .pConfig = NULL
};

// The following variables are stored in the ESP32's RTC RAM -
// their value is retained after a Sleep Reset.
RTC_DATA_ATTR uint32_t                        magicFlag1;               //!< flag for validating Session State in RTC RAM 
RTC_DATA_ATTR Arduino_LoRaWAN::SessionState   rtcSavedSessionState;     //!< Session State saved in RTC RAM
RTC_DATA_ATTR uint32_t                        magicFlag2;               //!< flag for validating Session Info in RTC RAM 
RTC_DATA_ATTR Arduino_LoRaWAN::SessionInfo    rtcSavedSessionInfo;      //!< Session Info saved in RTC RAM
RTC_DATA_ATTR size_t                          rtcSavedNExtraInfo;       //!< size of extra Session Info data
RTC_DATA_ATTR uint8_t                         rtcSavedExtraInfo[EXTRA_INFO_MEM_SIZE]; //!< extra Session Info data
RTC_DATA_ATTR bool                            runtimeExpired = false;   //!< flag indicating if runtime has expired at least once
RTC_DATA_ATTR bool                            longSleep;                //!< last sleep interval; 0 - normal / 1 - long
RTC_DATA_ATTR time_t                          rtcLastClockSync = 0;     //!< timestamp of last RTC synchonization to network time


#ifdef ADC_EN
    // ESP32 ADC with calibration
    ESP32AnalogRead adc; //!< ADC object for supply voltage measurement
#endif

// ESP32 ADC with calibration
#if defined(ADC_EN) && defined(PIN_ADC0_IN)
    ESP32AnalogRead adc0; //!< ADC object
#endif
#if defined(ADC_EN) && defined(PIN_ADC1_IN)
    ESP32AnalogRead adc1; //!< ADC object
#endif
#if defined(ADC_EN) && defined(PIN_ADC2_IN)
    ESP32AnalogRead adc2; //!< ADC object
#endif
#if defined(ADC_EN) && defined(PIN_ADC3_IN)
    ESP32AnalogRead adc3; //!< ADC object
#endif

/// Bresser Weather Sensor Receiver
WeatherSensor weatherSensor;

/// ESP32 preferences (stored in flash memory)
Preferences preferences;

struct sPrefs {
uint8_t   ws_timeout;           //!< preferences: weather sensor timeout
uint16_t  sleep_interval;       //!< preferences: sleep interval
uint16_t  sleep_interval_long;  //!< preferences: sleep interval long
} prefs;

#ifdef RAINDATA_EN
    /// Rain data statistics 
    RainGauge rainGauge;
#endif

#ifdef ONEWIRE_EN
    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire(PIN_ONEWIRE_BUS); //!< OneWire bus

    // Pass our oneWire reference to Dallas Temperature. 
    DallasTemperature temp_sensors(&oneWire); //!< Dallas temperature sensors connected to OneWire bus
#endif

#if defined(MITHERMOMETER_EN) || defined(THEENGSDECODER_EN)
    std::vector<std::string> knownBLEAddresses = KNOWN_BLE_ADDRESSES;
#endif

#ifdef DISTANCESENSOR_EN
    DistanceSensor_A02YYUW distanceSensor(&Serial2);
#endif

#ifdef MITHERMOMETER_EN
    // Setup BLE Temperature/Humidity Sensors
    ATC_MiThermometer bleSensors(knownBLEAddresses); //!< Mijia Bluetooth Low Energy Thermo-/Hygrometer
#endif
#ifdef THEENGSDECODER_EN
    BleSensors bleSensors(knownBLEAddresses);
#endif

/// LoRaWAN uplink payload buffer
static uint8_t loraData[PAYLOAD_SIZE]; //!< LoRaWAN uplink payload buffer

/// Sleep request (set in NetTxComplete();
bool sleepReq = false;

/// Force sleep mode after <code>sleepTimeout</code> has been reached (if FORCE_SLEEP is defined) 
ostime_t sleepTimeout; //!

/// Seconds since the UTC epoch
uint32_t userUTCTime;

/// RTC sync request flag - set (if due) in setup() / cleared in UserRequestNetworkTimeCb()
bool rtcSyncReq = false;

/// Real time clock
ESP32Time rtc;

/****************************************************************************\
|
|	Provisioning info for LoRaWAN OTAA
|
\****************************************************************************/

//
// For normal use, we require that you edit the sketch to replace FILLMEIN_x
// with values assigned by the your network. However, for regression tests,
// we want to be able to compile these scripts. The regression tests define
// COMPILE_REGRESSION_TEST, and in that case we define FILLMEIN_x to non-
// working but innocuous values.
//
// #define COMPILE_REGRESSION_TEST 1

#ifdef COMPILE_REGRESSION_TEST
# define FILLMEIN_8     1, 0, 0, 0, 0, 0, 0, 0
# define FILLMEIN_16    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2
#else
# warning "You must replace the values marked FILLMEIN with real values from the TTN control panel!"
# define FILLMEIN_8 (#dont edit this, edit the lines that use FILLMEIN_8)
# define FILLMEIN_16 (#dont edit this, edit the lines that use FILLMEIN_16)
#endif

// APPEUI, DEVEUI and APPKEY
#include "secrets.h"

#ifndef SECRETS
    #define SECRETS
    
    // The following constants should be copied to secrets.h and configured appropriately
    // according to the settings from TTN Console
    
    /// DeviceEUI, little-endian (lsb first)
    static const std::uint8_t deveui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    /// AppEUI, little-endian (lsb first)
    static const std::uint8_t appeui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    /// AppKey: just a string of bytes, sometimes referred to as "big endian".
    static const std::uint8_t appkey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#endif

/****************************************************************************\
|
|	setup()
|
\****************************************************************************/

/// Arduino setup
void setup() {
    // set baud rate
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    // wait for serial to be ready - or timeout if USB is not connected
    delay(500);

    preferences.begin("BWS-TTN", false);
    prefs.ws_timeout = preferences.getUChar("ws_timeout", WEATHERSENSOR_TIMEOUT);
    log_d("Preferences: weathersensor_timeout: %u s", prefs.ws_timeout);
    prefs.sleep_interval      = preferences.getUShort("sleep_interval", SLEEP_INTERVAL);
    log_d("Preferences: sleep_interval:        %u s", prefs.sleep_interval);
    prefs.sleep_interval_long = preferences.getUShort("sleep_interval_long", SLEEP_INTERVAL_LONG);
    log_d("Preferences: sleep_interval_long:   %u s", prefs.sleep_interval_long);
    preferences.end();
    
    sleepTimeout = sec2osticks(SLEEP_TIMEOUT_INITIAL);

    DEBUG_PRINTF_TS("");
    
    // Set time zone
    setenv("TZ", TZ_INFO, 1);
    printDateTime();
    
    // Check if clock was never synchronized or sync interval has expired 
    if ((rtcLastClockSync == 0) || ((rtc.getLocalEpoch() - rtcLastClockSync) > (CLOCK_SYNC_INTERVAL * 60))) {
        DEBUG_PRINTF("RTC sync required");
        rtcSyncReq = true;
    }

    // set up the log; do this first.
    myEventLog.setup();
    DEBUG_PRINTF("myEventlog.setup() - done");

    // set up the sensors.
    mySensor.setup();
    DEBUG_PRINTF("mySensor.setup() - done");

    // set up lorawan.
    myLoRaWAN.setup();
    DEBUG_PRINTF("myLoRaWAN.setup() - done");
    
    mySensor.uplinkRequest();
}

/****************************************************************************\
|
|	loop()
|
\****************************************************************************/

/// Arduino execution loop
void loop() {
    // the order of these is arbitrary, but you must poll them all.
    myLoRaWAN.loop();
    mySensor.loop();
    myEventLog.loop();

    #ifdef SLEEP_EN
        if (sleepReq & !rtcSyncReq) {
            myLoRaWAN.Shutdown();
            prepareSleep();
        }
    #endif

    #ifdef FORCE_SLEEP
        if (os_getTime() > sleepTimeout) {
            runtimeExpired = true;
            myLoRaWAN.Shutdown();
            #ifdef FORCE_JOIN_AFTER_SLEEP_TIMEOUT
                // Force join (instead of re-join)
                magicFlag1 = 0;
                magicFlag2 = 0;
            #endif
            DEBUG_PRINTF_TS("Sleep timer expired!");
            prepareSleep();
        }
    #endif
}

/****************************************************************************\
|
|	LoRaWAN methods
|
\****************************************************************************/

// Receive and process downlink messages
void ReceiveCb(
    void *pCtx,
    uint8_t uPort,
    const uint8_t *pBuffer,
    size_t nBuffer) {
            
    log_v("Port: %d", uPort);
    char buf[255];
    *buf = '\0';

    if (uPort > 0) {
        for (int i = 0; i < nBuffer; i++) {
              sprintf(&buf[strlen(buf)], "%02X ", pBuffer[i]);
        }
        log_v("Data: %s", buf);

        if ((pBuffer[0] == CMD_SET_DATETIME) && (nBuffer == 5)) {
            
            time_t set_time = pBuffer[4] | (pBuffer[3] << 8) | (pBuffer[2] << 16) | (pBuffer[1] << 24);
            rtc.setTime(set_time);
            rtcLastClockSync = rtc.getLocalEpoch();
            #if CORE_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
                char tbuf[25];
                struct tm timeinfo;
           
                localtime_r(&set_time, &timeinfo);
                strftime(tbuf, 25, "%Y-%m-%d %H:%M:%S", &timeinfo);
                log_d("Set date/time: %s", tbuf);
            #endif
        }
        if ((pBuffer[0] == CMD_SET_WEATHERSENSOR_TIMEOUT) && (nBuffer == 2)) {
            log_d("Set weathersensor_timeout: %u s", pBuffer[1]);
            preferences.begin("BWS-TTN", false);
            preferences.putUChar("ws_timeout", pBuffer[1]);
            preferences.end();
        }
        if ((pBuffer[0] == CMD_SET_SLEEP_INTERVAL) && (nBuffer == 3)){
            prefs.sleep_interval = pBuffer[2] | (pBuffer[1] << 8);
            log_d("Set sleep_interval: %u s", prefs.sleep_interval);
            preferences.begin("BWS-TTN", false);
            preferences.putUShort("sleep_interval", prefs.sleep_interval);
            preferences.end();            
        }
        if ((pBuffer[0] == CMD_SET_SLEEP_INTERVAL_LONG) && (nBuffer == 3)){
            prefs.sleep_interval_long = pBuffer[2] | (pBuffer[1] << 8);
            log_d("Set sleep_interval_long: %u s", prefs.sleep_interval_long);
            preferences.begin("BWS-TTN", false);
            preferences.putUShort("sleep_interval", prefs.sleep_interval_long);
            preferences.end();            
        }
        #ifdef RAINDATA_EN
            if ((pBuffer[0] == CMD_RESET_RAINGAUGE) && (nBuffer == 1)) {
                log_d("Reset raingauge");
                rainGauge.reset();
            }
        #endif
    }
}

// our setup routine does the class setup and then registers an event handler so
// we can see some interesting things
void
cMyLoRaWAN::setup() {
    // simply call begin() w/o parameters, and the LMIC's built-in
    // configuration for this board will be used.
    this->Super::begin(myPinMap);

//    LMIC_selectSubBand(0);
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);


    this->SetReceiveBufferBufferCb(ReceiveCb);
    
    this->RegisterListener(
        // use a lambda so we're "inside" the cMyLoRaWAN from public/private perspective
        [](void *pClientInfo, uint32_t event) -> void {
            auto const pThis = (cMyLoRaWAN *)pClientInfo;

            // for tx start, we quickly capture the channel and the RPS
            if (event == EV_TXSTART) {
                // use another lambda to make log prints easy
                myEventLog.logEvent(
                    (void *) pThis,
                    LMIC.txChnl,
                    LMIC.rps,
                    0,
                    // the print-out function
                    [](cEventLog::EventNode_t const *pEvent) -> void {
                        #if CORE_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
                            //rps_t _rps = rps_t(pEvent->getData(1));
                            //Serial.printf("rps (1): %02X\n", _rps);
                            uint8_t rps = pEvent->getData(1);
                            uint32_t tstamp = osticks2ms(pEvent->getTime());
                        #endif
                        // see MCCI_Arduino_LoRaWAN_Library/src/lib/arduino_lorawan_cEventLog.cpp
                        log_i("TX @%u ms: ch=%d rps=0x%02x (%s %s %s %s IH=%d)", 
                            tstamp,
                            std::uint8_t(pEvent->getData(0)),
                            rps,
                            myEventLog.getSfName(rps),
                            myEventLog.getBwName(rps),
                            myEventLog.getCrName(rps),
                            myEventLog.getCrcName(rps),
                            unsigned(getIh(rps)));      
                    }
                );
            }
            // else if (event == some other), record with print-out function
            else {
                // do nothing.
            }
        },
        (void *) this   // in case we need it.
        );
}

// this method is called when the LMIC needs OTAA info.
// return false to indicate "no provisioning", otherwise
// fill in the data and return true.
bool
cMyLoRaWAN::GetOtaaProvisioningInfo(
    OtaaProvisioningInfo *pInfo
    ) {
    // these are the same constants used in the LMIC compliance test script; eases testing
    // with the RedwoodComm RWC5020B/RWC5020M testers.

    // initialize info
    memcpy(pInfo->DevEUI, deveui, sizeof(pInfo->DevEUI));
    memcpy(pInfo->AppEUI, appeui, sizeof(pInfo->AppEUI));
    memcpy(pInfo->AppKey, appkey, sizeof(pInfo->AppKey));

    return true;
}

// This method is called after the node has joined the network.
void
cMyLoRaWAN::NetJoin(
    void) {
    DEBUG_PRINTF_TS("");
    sleepTimeout = os_getTime() + sec2osticks(SLEEP_TIMEOUT_JOINED);
    if (rtcSyncReq) {
        // Allow additional time for completing Network Time Request
        sleepTimeout += os_getTime() + sec2osticks(SLEEP_TIMEOUT_EXTRA);
    }
}

// This method is called after transmission has been completed.
// If enabled, the controller goes into deep sleep mode now.
void
cMyLoRaWAN::NetTxComplete(void) {
    DEBUG_PRINTF_TS("");
    sleepReq = true;
}

// Print session info for debugging
void 
cMyLoRaWAN::printSessionInfo(const SessionInfo &Info)
{
    log_v("Tag:\t\t%d", Info.V1.Tag);
    log_v("Size:\t\t%d", Info.V1.Size);
    log_v("Rsv2:\t\t%d", Info.V1.Rsv2);
    log_v("Rsv3:\t\t%d", Info.V1.Rsv3);
    log_v("NetID:\t\t0x%08X", Info.V1.NetID);
    log_v("DevAddr:\t\t0x%08X", Info.V1.DevAddr);
    if (CORE_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG) {
        char buf[64];
        *buf = '\0';
        for (int i=0; i<15;i++) {
            sprintf(&buf[strlen(buf)], "%02X ", Info.V1.NwkSKey[i]);
        }
        log_v("NwkSKey:\t\t%s", buf);
    }
    if (CORE_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG) {
        char buf[64];
        *buf = '\0';
        for (int i=0; i<15;i++) {
            sprintf(&buf[strlen(buf)], "%02X ", Info.V1.AppSKey[i]);  
        }
        log_v("AppSKey:\t\t%s", buf);
    }    
}

// Print session state for debugging
void 
cMyLoRaWAN::printSessionState(const SessionState &State)
{
    log_v("Tag:\t\t%d", State.V1.Tag);
    log_v("Size:\t\t%d", State.V1.Size);
    log_v("Region:\t\t%d", State.V1.Region);
    log_v("LinkDR:\t\t%d", State.V1.LinkDR);
    log_v("FCntUp:\t\t%d", State.V1.FCntUp);
    log_v("FCntDown:\t\t%d", State.V1.FCntDown);
    log_v("gpsTime:\t\t%d", State.V1.gpsTime);
    log_v("globalAvail:\t%d", State.V1.globalAvail);
    log_v("Rx2Frequency:\t%d", State.V1.Rx2Frequency);
    log_v("PingFrequency:\t%d", State.V1.PingFrequency);
    log_v("Country:\t\t%d", State.V1.Country);
    log_v("LinkIntegrity:\t%d", State.V1.LinkIntegrity);
    // There is more in it...
}


// Save Info to ESP32's RTC RAM
// if not possible, just do nothing and make sure you return false
// from NetGetSessionState().
void
cMyLoRaWAN::NetSaveSessionInfo(
    const SessionInfo &Info,
    const uint8_t *pExtraInfo,
    size_t nExtraInfo
    ) {
    if (nExtraInfo > EXTRA_INFO_MEM_SIZE)
        return;
    rtcSavedSessionInfo = Info;
    rtcSavedNExtraInfo = nExtraInfo;
    memcpy(rtcSavedExtraInfo, pExtraInfo, nExtraInfo);
    magicFlag2 = MAGIC2;
    DEBUG_PRINTF_TS("");
    printSessionInfo(Info);
}


// Save State in RTC RAM. Note that it's often the same;
// often only the frame counters change.
// [If not possible, just do nothing and make sure you return false
// from NetGetSessionState().]
void
cMyLoRaWAN::NetSaveSessionState(const SessionState &State) {
    rtcSavedSessionState = State;
    magicFlag1 = MAGIC1;
    DEBUG_PRINTF_TS("");
    printSessionState(State);
}

// Either fetch SessionState from somewhere and return true or...
// return false, which forces a re-join.
bool
cMyLoRaWAN::NetGetSessionState(SessionState &State) {
    if (magicFlag1 == MAGIC1) {
        State = rtcSavedSessionState;
        DEBUG_PRINTF_TS("o.k.");
        printSessionState(State);
        return true;
    } else {
        DEBUG_PRINTF_TS("failed");
        return false;
    }
}

// Get APB provisioning info - this is also used in OTAA after a succesful join.
// If it can be provided in OTAA mode after a restart, no re-join is needed.
bool
cMyLoRaWAN::GetAbpProvisioningInfo(AbpProvisioningInfo *pAbpInfo) {
    SessionState state;

    // ApbInfo:
    // --------
    // uint8_t         NwkSKey[16];
    // uint8_t         AppSKey[16];
    // uint32_t        DevAddr;
    // uint32_t        NetID;
    // uint32_t        FCntUp;
    // uint32_t        FCntDown;
    
    if ((magicFlag1 != MAGIC1) || (magicFlag2 != MAGIC2)) {
         return false;
    }
    DEBUG_PRINTF_TS("");

    pAbpInfo->DevAddr = rtcSavedSessionInfo.V2.DevAddr;
    pAbpInfo->NetID   = rtcSavedSessionInfo.V2.NetID;
    memcpy(pAbpInfo->NwkSKey, rtcSavedSessionInfo.V2.NwkSKey, 16);
    memcpy(pAbpInfo->AppSKey, rtcSavedSessionInfo.V2.AppSKey, 16);
    NetGetSessionState(state);
    pAbpInfo->FCntUp   = state.V1.FCntUp;
    pAbpInfo->FCntDown = state.V1.FCntDown;

    if (CORE_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG) {
        char buf[64];
        
        *buf = '\0';
        for (int i=0; i<15;i++) {
          sprintf(&buf[strlen(buf)], "%02X ", pAbpInfo->NwkSKey[i]);  
        }
        log_v("NwkSKey:\t%s", buf);
        
        *buf = '\0';
        for (int i=0; i<15;i++) {
          sprintf(&buf[strlen(buf)], "%02X ", pAbpInfo->AppSKey[i]);  
        }
        log_v("AppSKey:\t%s", buf);
        log_v("FCntUp:\t%d", state.V1.FCntUp);
    }
    return true;
}

/// Print date and time (i.e. local time)
void printDateTime(void) {
        struct tm timeinfo;
        char tbuf[26];
        
        time_t tnow = rtc.getLocalEpoch();
        localtime_r(&tnow, &timeinfo);
        strftime(tbuf, 25, "%Y-%m-%d %H:%M:%S", &timeinfo);
        DEBUG_PRINTF("%s", tbuf);
}

/// Determine sleep duration and enter Deep Sleep Mode
void prepareSleep(void) {
    uint32_t sleep_interval = prefs.sleep_interval;
    longSleep = false;
    #ifdef ADC_EN
        // Long sleep interval if battery is weak
        if (mySensor.getVoltage() < BATTERY_WEAK) {
            sleep_interval = prefs.sleep_interval_long;
            longSleep = true;
        }
    #endif

    // If the real time is available, align the wake-up time to the
    // to next non-fractional multiple of sleep_interval past the hour
    if (rtcLastClockSync) {
        struct tm timeinfo;
        time_t t_now = rtc.getLocalEpoch();
        localtime_r(&t_now, &timeinfo);

        sleep_interval = sleep_interval - ((timeinfo.tm_min * 60) % sleep_interval + timeinfo.tm_sec);
        sleep_interval += 20; // Added extra 20-secs of sleep to allow for slow ESP32 RTC timers
    }
    
    DEBUG_PRINTF_TS("Shutdown() - sleeping for %d s", sleep_interval);
    ESP.deepSleep(sleep_interval * 1000000LL);
}

/**
 * \fn UserRequestNetworkTimeCb
 * 
 * \brief Callback function for setting RTC from LoRaWAN network time
 * 
 * \param pVoidUserUTCTime user supplied buffer for UTC time
 * 
 * \param flagSuccess flag indicating if network time request was succesful
 */
void UserRequestNetworkTimeCb(void *pVoidUserUTCTime, int flagSuccess) {
    // Explicit conversion from void* to uint32_t* to avoid compiler errors
    uint32_t *pUserUTCTime = (uint32_t *) pVoidUserUTCTime;

    // A struct that will be populated by LMIC_getNetworkTimeReference.
    // It contains the following fields:
    //  - tLocal: the value returned by os_GetTime() when the time
    //            request was sent to the gateway, and
    //  - tNetwork: the seconds between the GPS epoch and the time
    //              the gateway received the time request
    lmic_time_reference_t lmicTimeReference;

    if (flagSuccess != 1) {
        // Most likely the service is not provided by the gateway. No sense in trying again...
        DEBUG_PRINTF_TS("didn't succeed");
        rtcSyncReq = false;
        return;
    }

    // Populate "lmic_time_reference"
    flagSuccess = LMIC_getNetworkTimeReference(&lmicTimeReference);
    if (flagSuccess != 1) {
        DEBUG_PRINTF_TS("LMIC_getNetworkTimeReference didn't succeed");
        return;
    }

    // Update userUTCTime, considering the difference between the GPS and UTC
    // epoch, and the leap seconds
    *pUserUTCTime = lmicTimeReference.tNetwork + 315964800;

    // Add the delay between the instant the time was transmitted and
    // the current time

    // Current time, in ticks
    ostime_t ticksNow = os_getTime();
    // Time when the request was sent, in ticks
    ostime_t ticksRequestSent = lmicTimeReference.tLocal;
    uint32_t requestDelaySec = osticks2ms(ticksNow - ticksRequestSent) / 1000;
    *pUserUTCTime += requestDelaySec;

    // Update the system time with the time read from the network
    rtc.setTime(*pUserUTCTime);
    
    // Save clock sync timestamp and clear flag 
    rtcLastClockSync = rtc.getLocalEpoch();
    rtcSyncReq = false;
    DEBUG_PRINTF_TS("RTC sync completed");
    printDateTime();
}

void
cMyLoRaWAN::requestNetworkTime(void) {
    LMIC_requestNetworkTime(UserRequestNetworkTimeCb, &userUTCTime);
}

/****************************************************************************\
|
|	Sensor methods
|
\****************************************************************************/

void
cSensor::setup(std::uint32_t uplinkPeriodMs) {
    // set the initial time.
    this->m_uplinkPeriodMs = uplinkPeriodMs;
    this->m_tReference = millis();

    #ifdef DISTANCESENSOR_EN
        Serial2.begin(9600, SERIAL_8N1, DISTANCESENSOR_RX, DISTANCESENSOR_TX);
        pinMode(DISTANCESENSOR_PWR, OUTPUT);
        digitalWrite(DISTANCESENSOR_PWR, LOW);
    #endif

    #ifdef ADC_EN
        // Use ADC with PIN_ADC_IN
        adc.attach(PIN_ADC_IN);
        
        if (getVoltage() < BATTERY_LOW) {
          DEBUG_PRINTF("Battery low!");
          prepareSleep();
        }
    #endif

    #if defined(ADC_EN) && defined(PIN_ADC3_IN)
        // Use ADC3 with PIN_ADC3_IN
        adc3.attach(PIN_ADC3_IN);
    #endif
    
    #if defined(MITHERMOMETER_EN) || defined(THEENGSDECODER_EN)
        bleSensors.begin();
    #endif
    
    #ifndef LORAWAN_DEBUG
        weatherSensor.begin();
        //bool decode_ok = weatherSensor.getData(prefs.ws_timeout * 1000, DATA_TYPE | DATA_COMPLETE, SENSOR_TYPE_WEATHER1);
        bool decode_ok = weatherSensor.getData(prefs.ws_timeout * 1000, DATA_ALL_SLOTS);
    #else
        bool decode_ok = weatherSensor.genMessage(0 /* slot */, 0x01234567 /* ID */, 1 /* type */, 0 /* channel */);
    #endif
    if (decode_ok) {
        DEBUG_PRINTF("Receiving Weather Sensor Data o.k.");
    } else {
        DEBUG_PRINTF("Receiving Weather Sensor Data failed.");
        #ifdef WEATHERSENSOR_DATA_REQUIRED
            prepareSleep();
        #endif
    }
    
    #ifdef RAINDATA_EN
        // Check if time is valid
        if (rtcLastClockSync > 0) {
            // Get local date and time
            struct tm timeinfo;
            time_t tnow = rtc.getLocalEpoch();
            localtime_r(&tnow, &timeinfo);

            // Find weather sensor and determine rain gauge overflow limit
            uint32_t rg_overflow;
            // Try to find SENSOR_TYPE_WEATHER0
            int ws = weatherSensor.findType(SENSOR_TYPE_WEATHER0);
            if (ws > -1) {
                rg_overflow = 100;
            }
            else {
                // Try to find SENSOR_TYPE_WEATHER1
                ws = weatherSensor.findType(SENSOR_TYPE_WEATHER1);
                rg_overflow = 100000;
            }

            // If weather sensor has be found and rain data is valid, update statistics
            if ((ws > -1) && weatherSensor.sensor[ws].valid && weatherSensor.sensor[ws].rain_ok) {
                rainGauge.update(timeinfo, weatherSensor.sensor[ws].rain_mm, rg_overflow);
            }
        }
    #endif
}


void
cSensor::loop(void) {
    auto const tNow = millis();
    auto const deltaT = tNow - this->m_tReference;

    if (deltaT >= this->m_uplinkPeriodMs) {
        // request an uplink
        this->m_fUplinkRequest = true;

        // keep trigger time locked to uplinkPeriod
        auto const advance = deltaT / this->m_uplinkPeriodMs;
        this->m_tReference += advance * this->m_uplinkPeriodMs; 
    }

    // if an uplink was requested, do it.
    if (this->m_fUplinkRequest) {
        this->m_fUplinkRequest = false;
        this->doUplink();
    }
}

#ifdef ADC_EN
//
// Get supply / battery voltage
//
uint16_t
cSensor::getVoltage(void)
{
    float voltage_raw = 0;
    for (uint8_t i=0; i < UBATT_SAMPLES; i++) {
        voltage_raw += float(adc.readMiliVolts());
    }
    uint16_t voltage = int(voltage_raw / UBATT_SAMPLES / UBATT_DIV);
     
    DEBUG_PRINTF("Voltage = %dmV", voltage);

    return voltage;
}

//
// Get supply / battery voltage
//
uint16_t
cSensor::getVoltage(ESP32AnalogRead &adc, uint8_t samples, float divider)
{
    float voltage_raw = 0;
    for (uint8_t i=0; i < samples; i++) {
        voltage_raw += float(adc.readMiliVolts());
    }
    uint16_t voltage = int(voltage_raw / samples / divider);
     
    DEBUG_PRINTF("Voltage = %dmV", voltage);

    return voltage;
}
#endif

#ifdef ONEWIRE_EN
//
// Get temperature from Maxim OneWire Sensor
//
float
cSensor::getTemperature(void)
{
    // Call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    temp_sensors.requestTemperatures(); // Send the command to get temperatures
        
    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    float tempC = temp_sensors.getTempCByIndex(0);

    #ifdef _DEBUG_MODE_
        // Check if reading was successful
        if (tempC != DEVICE_DISCONNECTED_C) {
            DEBUG_PRINTF("Temperature = %.2f°C", tempC);
        } else {
            DEBUG_PRINTF("Error: Could not read temperature data");
        }
    #endif
    return tempC;
}
#endif
    
//
// Prepare uplink data for transmission
//
// Note:
// Currently Weather Sensor Data can only be received during initialization if the radio transceiver
// is shared between BresserWeatherSensorReceiver and LoRaWAN library.
// Therefore the node must perform a restart (from deep-sleep) after each succesful uplink.
//
// The required procedure to update the Weather Sensor Data (if radio receiver shared with LoRaWAN)
// without deep sleep mode and restart would be:
// 1. Save radio registers modified by LoRaWAN
// 2. Re-initialize radio for sensor data reception (FSK mode)
// 3. Receive data
// 4. Restore radio registers modified by LoRaWAN
//
// ToDo:
// Which registers are actually shared between FSK and LoRaWAN mode?
// Is there any additional state to be preserved?
void
cSensor::doUplink(void) {
    // if busy uplinking, just skip
    if (this->m_fBusy) {
        DEBUG_PRINTF_TS("busy");
        return;
    }
    // if LMIC is busy, just skip
    if (LMIC.opmode & (OP_POLL | OP_TXDATA | OP_TXRXPEND)) {
        DEBUG_PRINTF_TS("other operation in progress");    
        return;
    }

    //
    // Request time and date
    //
    if (rtcSyncReq)
        myLoRaWAN.requestNetworkTime();
    
    //
    // Read auxiliary sensor data
    //
    #ifdef ONEWIRE_EN
        float     water_temp_c        = getTemperature();
    #endif
    #ifdef DISTANCESENSOR_EN
        // Sensor power on
        digitalWrite(DISTANCESENSOR_PWR, HIGH);
        delay(500);
        
        int retries = 0;
        DistanceSensor_A02YYUW_MEASSUREMENT_STATUS dstStatus;
        do {
            dstStatus = distanceSensor.meassure();

            if (dstStatus != DistanceSensor_A02YYUW_MEASSUREMENT_STATUS_OK) {
                DEBUG_PRINTF("Distance Sensor Error: %d", dstStatus);
            }
        } while (
            (dstStatus != DistanceSensor_A02YYUW_MEASSUREMENT_STATUS_OK) &&
            (++retries < DISTANCESENSOR_RETRIES)
        );
        
        uint16_t  distance_mm;
        if (dstStatus == DistanceSensor_A02YYUW_MEASSUREMENT_STATUS_OK) {
            distance_mm = distanceSensor.getDistance();
        } else {
            distance_mm = 0;
        }
        
        // Sensor power off
        digitalWrite(DISTANCESENSOR_PWR, LOW);
    #endif
    #ifdef ADC_EN
        uint16_t  supply_voltage      = getVoltage();
    #endif
    #if defined(ADC_EN) && defined(PIN_ADC3_IN)
        uint16_t  battery_voltage     = getVoltage(adc3, ADC3_SAMPLES, ADC3_DIV);
    #endif
    bool          mithermometer_valid = false;
    #if defined(MITHERMOMETER_EN) || defined(THEENGSDECODER_EN) 
        float     indoor_temp_c;
        float     indoor_humidity;
    
        // Set sensor data invalid
        bleSensors.resetData();
        
        // Get sensor data - run BLE scan for <bleScanTime>
        bleSensors.getData(BLE_SCAN_TIME);
    #endif
    //
    // Find Bresser sensor data in array 
    //
    
    // Try to find SENSOR_TYPE_WEATHER0
    int ws = weatherSensor.findType(SENSOR_TYPE_WEATHER0);
    if (ws < 0) {
      // Try to find SENSOR_TYPE_WEATHER1
      ws = weatherSensor.findType(SENSOR_TYPE_WEATHER1);
    }

    int s1 = -1;
    #ifdef SOILSENSOR_EN
      // Try to find SENSOR_TYPE_SOIL
      s1 = weatherSensor.findType(SENSOR_TYPE_SOIL, 1);
    #endif

    int ls = -1;
    #ifdef LIGHTNINGSENSOR_EN
      // Try to find SENSOR_TYPE_LIGHTNING
      ls = weatherSensor.findType(SENSOR_TYPE_LIGHTNING);
    #endif
    
    DEBUG_PRINTF("--- Uplink Data ---");
    
    // Debug output for weather sensor data
    if (ws > -1) {
      DEBUG_PRINTF("Air Temperature:    % 3.1f °C",   weatherSensor.sensor[ws].temp_c);
      DEBUG_PRINTF("Humidity:            %2d   %%",   weatherSensor.sensor[ws].humidity);
      DEBUG_PRINTF("Rain Gauge:       %7.1f mm",      weatherSensor.sensor[ws].rain_mm);
      DEBUG_PRINTF("Wind Speed (avg.):    %3.1f m/s", weatherSensor.sensor[ws].wind_avg_meter_sec_fp1/10.0);
      DEBUG_PRINTF("Wind Speed (max.):    %3.1f m/s", weatherSensor.sensor[ws].wind_gust_meter_sec_fp1/10.0);
      DEBUG_PRINTF("Wind Direction:     %4.1f °",     weatherSensor.sensor[ws].wind_direction_deg_fp1/10.0);
    } else {
      DEBUG_PRINTF("-- Weather Sensor Failure");
    }

    // Debug output for soil sensor data
    #ifdef SOILSENSOR_EN
      if (s1 > -1) {
        DEBUG_PRINTF("Soil Temperature 1: %3.1f °C",  weatherSensor.sensor[s1].temp_c);
        DEBUG_PRINTF("Soil Moisture 1:     %2d   %%",  weatherSensor.sensor[s1].moisture);      
      } else {
        DEBUG_PRINTF("-- Soil Sensor 1 Failure");
      }
    #endif

    // Debug output for lightning sensor data
    #ifdef LIGHTNINGSENSOR_EN
      if (ls > -1) {
        DEBUG_PRINTF("Lightning counter:  %3d",  weatherSensor.sensor[ls].lightning_count);
        DEBUG_PRINTF("Lightning distance:  %2d   km",  weatherSensor.sensor[ls].lightning_distance_km);      
      } else {
        DEBUG_PRINTF("-- Lightning Sensor Failure");
      }
    #endif
    
    #ifdef ONEWIRE_EN
        // Debug output for auxiliary sensors/voltages
        if (water_temp_c != DEVICE_DISCONNECTED_C) {
            DEBUG_PRINTF("Water Temperature:  % 2.1f °C",  water_temp_c);
        } else {
            DEBUG_PRINTF("Water Temperature:   --.- °C");
            water_temp_c = -30.0;
        }
    #endif
    #ifdef DISTANCESENSOR_EN
        if (distance_mm > 0) {
            DEBUG_PRINTF("Distance:          %4d mm", distance_mm);
        } else {
            DEBUG_PRINTF("Distance:         ---- mm");
        }
    #endif
    #ifdef ADC_EN
        DEBUG_PRINTF("Supply  Voltage:   %4d   mV",       supply_voltage);
    #endif
    #if defined(ADC_EN) && defined(PIN_ADC3_IN)
        DEBUG_PRINTF("Battery Voltage:   %4d   mV",       battery_voltage);
    #endif
    
    #if defined(MITHERMOMETER_EN)
        float div = 100.0;
    #elif defined(THEENGSDECODER_EN)
        float div = 1.0;
    #endif
    #if defined(MITHERMOMETER_EN) || defined(THEENGSDECODER_EN)
        if (bleSensors.data[0].valid) {
            mithermometer_valid = true;
            indoor_temp_c   = bleSensors.data[0].temperature/div;
            indoor_humidity = bleSensors.data[0].humidity/div;
            DEBUG_PRINTF("Indoor Air Temp.:   % 3.1f °C", bleSensors.data[0].temperature/div);
            DEBUG_PRINTF("Indoor Humidity:     %3.1f %%", bleSensors.data[0].humidity/div);
        } else {
            DEBUG_PRINTF("Indoor Air Temp.:    --.- °C");
            DEBUG_PRINTF("Indoor Humidity:     --   %%");
            indoor_temp_c   = -30;
            indoor_humidity = 0;
        }
    #endif
    DEBUG_PRINTF("");

    //
    // Encode sensor data as byte array for LoRaWAN transmission
    //
    LoraEncoder encoder(loraData);
    #ifdef SENSORID_EN
        if (ws > -1) {
          encoder.writeUint32(weatherSensor.sensor[ws].sensor_id);
        } else {
          encoder.writeUint32(0);
        }
    #endif

    // TTN node status flags
    encoder.writeBitmap(0,
                        0,
                        0,
                        0,
                        0,
                        longSleep,
                        rtcSyncReq, 
                        runtimeExpired);

    // Sensor status flags
    encoder.writeBitmap(0,
                        mithermometer_valid,
                        (ls > -1) ? weatherSensor.sensor[ls].valid : false,
                        (ls > -1) ? weatherSensor.sensor[ls].battery_ok : false,
                        (s1 > -1) ? weatherSensor.sensor[s1].valid : false,
                        (s1 > -1) ? weatherSensor.sensor[s1].battery_ok : false,
                        (ws > -1) ? weatherSensor.sensor[ws].valid : false,
                        (ws > -1) ? weatherSensor.sensor[ws].battery_ok : false);
    
    // Weather sensor data
    if (ws > -1) {
        // weather sensor data available
        encoder.writeTemperature(weatherSensor.sensor[ws].temp_c);
        encoder.writeUint8(weatherSensor.sensor[ws].humidity);
        #ifdef ENCODE_AS_FLOAT
            encoder.writeRawFloat(weatherSensor.sensor[ws].wind_gust_meter_sec);
            encoder.writeRawFloat(weatherSensor.sensor[ws].wind_avg_meter_sec);
            encoder.writeRawFloat(weatherSensor.sensor[ws].wind_direction_deg);
        #else
            encoder.writeUint16(weatherSensor.sensor[ws].wind_gust_meter_sec_fp1);
            encoder.writeUint16(weatherSensor.sensor[ws].wind_avg_meter_sec_fp1);
            encoder.writeUint16(weatherSensor.sensor[ws].wind_direction_deg_fp1);
        #endif
        encoder.writeRawFloat(weatherSensor.sensor[ws].rain_mm);
    } else {
        // fill with suspicious dummy values
        encoder.writeTemperature(-30);
        encoder.writeUint8(0);
        #ifdef ENCODE_AS_FLOAT
            encoder.writeRawFloat(0);
            encoder.writeRawFloat(0);
            encoder.writeRawFloat(0);
        #else
            encoder.writeUint16(0);
            encoder.writeUint16(0);
            encoder.writeUint16(0);
        #endif
        encoder.writeRawFloat(0);
    }

    // Voltages / auxiliary sensor data
    #ifdef ADC_EN
        encoder.writeUint16(supply_voltage);
    #endif
    #if defined(ADC_EN) && defined(PIN_ADC3_IN)
        encoder.writeUint16(battery_voltage);
    #endif
    #ifdef ONEWIRE_EN
        encoder.writeTemperature(water_temp_c);
    #endif
    #if defined(MITHERMOMETER_EN) || defined(THEENGSDECODER_EN)
        encoder.writeTemperature(indoor_temp_c);
        encoder.writeUint8((uint8_t)(indoor_humidity+0.5));

        // BLE Tempoerature/Humidity Sensor: delete results fromBLEScan buffer to release memory
        bleSensors.clearScanResults();
    #endif    

    // Soil sensor data
    #ifdef SOILSENSOR_EN
        if (s1 > -1) {
            // soil sensor data available
            encoder.writeTemperature(weatherSensor.sensor[s1].temp_c);
            encoder.writeUint8(weatherSensor.sensor[s1].moisture);
        } else {
            // fill with suspicious dummy values
            encoder.writeTemperature(-30);
            encoder.writeUint8(0);
        }
    #endif

    // Rain data statistics
    #ifdef RAINDATA_EN
        if ((ws > -1) && weatherSensor.sensor[ws].valid && weatherSensor.sensor[ws].rain_ok) {
            DEBUG_PRINTF("Rain past 60min:  %7.1f mm", rainGauge.pastHour());
            DEBUG_PRINTF("Rain curr. day:   %7.1f mm", rainGauge.currentDay());
            DEBUG_PRINTF("Rain curr. week:  %7.1f mm", rainGauge.currentWeek());
            DEBUG_PRINTF("Rain curr. month: %7.1f mm", rainGauge.currentMonth());
            encoder.writeRawFloat(rainGauge.pastHour());
            encoder.writeRawFloat(rainGauge.currentDay());
            encoder.writeRawFloat(rainGauge.currentWeek());
            encoder.writeRawFloat(rainGauge.currentMonth());
        } else {
            DEBUG_PRINTF("Current rain gauge statistics not valid.");
            encoder.writeRawFloat(-1);
            encoder.writeRawFloat(-1);
            encoder.writeRawFloat(-1);
            encoder.writeRawFloat(-1);            
        }
    #endif

    // Distance sensor data
    #ifdef DISTANCESENSOR_EN
        encoder.writeUint16(distance_mm);
    #endif

    // Lightning sensor data
    #ifdef LIGHTNINGSENSOR_EN
        if (ls > -1) {
            // Lightning sensor data available
            encoder.writeUint16(weatherSensor.sensor[ls].lightning_count);
            encoder.writeUint8(weatherSensor.sensor[ls].lightning_distance_km);
        } else {
            // Fill with suspicious dummy values
            encoder.writeUint16(-1);
            encoder.writeUint16(-1);
        }
    #endif
    //encoder.writeRawFloat(radio.getRSSI()); // NOTE: int8_t would be more efficient

    this->m_fBusy = true;

    // Schedule transmission
    if (! myLoRaWAN.SendBuffer(
        loraData, encoder.getLength(),
        // this is the completion function:
        [](void *pClientData, bool fSucccess) -> void {
            auto const pThis = (cSensor *)pClientData;
            pThis->m_fBusy = false;
        },
        (void *)this,
        /* confirmed */ true,
        /* port */ 1
        )) {
        // sending failed; callback has not been called and will not
        // be called. Reset busy flag.
        this->m_fBusy = false;
    }
}
