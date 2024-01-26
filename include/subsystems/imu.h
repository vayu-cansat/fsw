#pragma once
#include <Arduino.h>
#include "variables.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void imu_setup()
{
#ifdef IMU_ACTIVE
    if (!mpu.begin(0x69))
    {
        Serial.println("❌ MPU6050");
    }
    else
    {
        Serial.println("✅ MPU6050 Found!");
    }
#endif
}

void imu_loop()
{
#ifdef IMU_ACTIVE
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    tilt_x = g.gyro.x;
    tilt_y = g.gyro.y;
    rot_z = g.gyro.z;
#endif
}