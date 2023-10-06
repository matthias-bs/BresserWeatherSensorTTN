#if defined(ARDUINO_ARCH_RP2040)
#include <Arduino.h>
#include <time.h>
#include "pico_sleep.h"
#include "pico_rosc.h"

#ifndef PICO_RTC_UTILS_H
#define PICO_RTC_UTILS_H

struct tm *datetime_to_tm(datetime_t *dt, struct tm *ti);
datetime_t *tm_to_datetime(struct tm *ti, datetime_t *dt);

void print_dt(datetime_t dt);
void print_tm(struct tm ti);

void pico_sleep(unsigned duration);

#endif // PICO_RTC_UTILS_H
#endif // defined(ARDUINO_ARCH_RP2040)
