#pragma once
#include <Arduino.h>
#include "variables.h"
#include <MPU6050_tockn.h>

MPU6050 mpu(Wire);

void imu_setup()
{
#ifdef IMU_ACTIVE
    if (!mpu.begin())
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
    mpu.update();
    tilt_x = mpu.getAngleX();
    tilt_y = mpu.getAngleY();
    rot_z = mpu.getAngleZ();
    temperature = mpu.getTemp();
#endif
}

void imu_calibrate()
{
    mpu.calcGyroOffsets(false, 1000, 1000);
    debug_message = "Calibrated IMU!";
}