# Debug Output Configuration in Arduino IDE

## ESP32

1. Select appropriate (USB-)serial port for your board
![Arduino_IDE-Tools_Port](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/be496bf8-89ce-4db5-b1bf-c88a7f5e99cb)
![Arduino_IDE-Select_Other_Board_and_Port](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/99ab22f6-5c0f-4cc7-a8dc-6172376338c7)

  <img src="https://user-images.githubusercontent.com/83612361/210953111-3381fcb4-0d39-43f5-bcfe-5d381bc04c00.png" alt="ESP32 Port Selection" width="500">
![Arduino_IDE-Tools_CoreDebugLevel](https://github.com/matthias-bs/BresserWeatherSensorTTN/assets/83612361/72a8b1d9-8d39-41fc-9658-78b432b73d56)

2. Select desired debug level
   
  <img src="https://user-images.githubusercontent.com/83612361/210953437-b35a0bb3-df71-4292-ad0b-9e50846d6cc8.png" alt="ESP32 Core Debug Level" width="500">

  This passes the define `CORE_DEBUG_LEVEL`to the compiler accordingly.

Refer to the following for some background information
* https://thingpulse.com/esp32-logging/
* https://www.mischianti.org/2020/09/20/esp32-manage-multiple-serial-and-logging-for-debugging-3/
* https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-log.h
