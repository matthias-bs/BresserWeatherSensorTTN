If you cannot use the default configuration and you do not want do change WeatherSensorCfg.h in the Arduino library directory (Arduino/libraries/BresserWeatherSensorReceiver/src/WeatherSensorCfg.h):
* Copy the directory `BresserWeatherSensorReceiver` here
* customize `./BresserWeatherSensorReceiver/src/WeatherSensorCfg.h` - see [WeatherSensorCfg.h.template](WeatherSensorCfg.h.template) for settings consistent with [BresserWeatherSensorTTN.ino](../BresserWeatherSensorTTN.ino)
* change the include directives in `BresserWeatherSensorTTN.ino`

   from
   ```
   // BresserWeatherSensorReceiver
   #include "WeatherSensorCfg.h"
   #include "WeatherSensor.h"
   ```
   to
   ```
   // BresserWeatherSensorReceiver
   #include "src/BresserWeatherSensorReceiver/src/WeatherSensorCfg.h"
   #include "src/BresserWeatherSensorReceiver/src/WeatherSensor.h"
   ```
