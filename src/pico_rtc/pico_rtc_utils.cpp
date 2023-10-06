#if defined(ARDUINO_ARCH_RP2040)

#include "pico_rtc_utils.h"

struct tm *datetime_to_tm(datetime_t *dt, struct tm *ti)
{
  ti->tm_sec = dt->sec;
  ti->tm_min = dt->min;
  ti->tm_hour = dt->hour;
  ti->tm_mday = dt->day;
  ti->tm_mon = dt->month - 1;
  ti->tm_year = dt->year - 1900;
  ti->tm_wday = dt->dotw;

  return ti;
}

datetime_t *tm_to_datetime(struct tm *ti, datetime_t *dt)
{
  dt->sec = ti->tm_sec;
  dt->min = ti->tm_min;
  dt->hour = ti->tm_hour;
  dt->day = ti->tm_mday;
  dt->month = ti->tm_mon + 1;
  dt->year = ti->tm_year + 1900;
  dt->dotw = ti->tm_wday;

  return dt;
}

void print_dt(datetime_t dt) {
  printf("%4d-%02d-%02d %02d:%02d:%02d\n", dt.year, dt.month, dt.day, dt.hour, dt.min, dt.sec);
}

void print_tm(struct tm ti) {
    printf("%4d-%02d-%02d %02d:%02d:%02d\n", ti.tm_year+1900, ti.tm_mon+1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec);
}

void pico_sleep(unsigned duration) {
    datetime_t dt;
    rtc_get_datetime(&dt);
    printf("RTC time:\n");
    print_dt(dt);

    struct tm ti;
    datetime_to_tm(&dt, &ti);

    // Convert to epoch
    time_t now = mktime(&ti);
    
    // Add sleep_duration
    time_t wakeup = now + duration;

    // Convert epoch to struct tm
    localtime_r(&wakeup, &ti);

    // Convert struct tm to datetime_t
    tm_to_datetime(&ti, &dt);
    printf("Wakeup time:\n");
    print_dt(dt);

    Serial.flush();
    Serial1.end();
    Serial2.end();
    Serial.end();

    // From
    // https://github.com/lyusupov/SoftRF/tree/master/software/firmware/source/libraries/RP2040_Sleep
    // also see src/pico_rtc
    // --8<-----
    #if defined(USE_TINYUSB)
        // Disable USB
        USBDevice.detach();
    #endif /* USE_TINYUSB */

    sleep_run_from_xosc();

    sleep_goto_sleep_until(&dt, NULL);

    // back from dormant state
    rosc_enable();
    clocks_init();
    // --8<-----
}
#endif