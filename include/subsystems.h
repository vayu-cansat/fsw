#pragma once

// Just comment out the subsystems we don't want to use during development
#define COM_ACTIVE
#define IMU_ACTIVE
#define GPS_ACTIVE
#define RTC_ACTIVE
#define GPS_ACTIVE
#define BMP_ACTIVE

#include "comms.h"
#include "subsystems/rtc.h"
#include "subsystems/imu.h"
#include "subsystems/gps.h"
#include "subsystems/bmp.h"