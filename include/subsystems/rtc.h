#pragma once
#include <Arduino.h>
#include "variables.h"
#include <RTClib.h>

RTC_DS3231 rtc;

void rtc_setup()
{
#ifdef RTC_ACTIVE
    if (!rtc.begin())
    {
        Serial.println("❌ RTC");
    }
    else
    {
        Serial.println("✅ RTC Found!");
    }
    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
#endif
}

void rtc_loop()
{
#ifdef RTC_ACTIVE
    DateTime now = rtc.now();
    mission_time = (String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second();
#endif
}