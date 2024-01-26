#pragma once
#include <Arduino.h>
#include "variables.h"
#include <RTClib.h>

RTC_DS3231 rtc;

void rtc_setup()
{
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
    }
    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
}

void rtc_loop()
{
    DateTime now = rtc.now();
    mission_time = (String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second();
}