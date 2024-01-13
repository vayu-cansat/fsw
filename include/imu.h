// #pragma once
// // #include "basicMPU6050.h"
// // #include <EEPROM.h>
// #include "variables.h"

// basicMPU6050<> mpu;

// float rateRoll, ratePitch, rateYaw;
// float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
// int RateCalibrationNumber;
// float AngleRoll, AnglePitch, AngleYaw;
// uint32_t LoopTimer;
// float KalmanAngleRoll = 0, KalmanAnglePitch;
// float KalmanUncertaintyRoll = 2 * 2, KalmanUncertaintyPitch = 2 * 2;
// float Kalman1DOutput[] = {0, 0};

// int imu_prev;

// void store_bias()
// {
//     // float bx = mpu.getBiasX();
//     // float by = mpu.getBiasY();
//     // float bz = mpu.getBiasZ();
//     // // save to eeprom
//     // EEPROM.write(0, bx);
//     // EEPROM.write(1, by);
//     // EEPROM.write(2, bz);
// }

// void imu_calibrate()
// {
//     debug_message = "Calibrating IMU";
//     mpu.setBias();
//     store_bias();
//     debug_message = "IMU Calibrated";
// }

// void imu_setup()
// {
//     mpu.setup();
// #ifdef IMU_CALIBRATE_ON_BOOT
//     imu_calibrate();
// #else
//     float bx = EEPROM.read(0);
//     float by = EEPROM.read(1);
//     float bz = EEPROM.read(2);
//     mpu.setBiasX(bx);
//     mpu.setBiasY(by);
//     mpu.setBiasZ(bz);
// #endif

//     imu_prev = micros();
// }

// void imu_loop()
// {
//     int interval = micros() - imu_prev;
//     if (interval < 3000)
//         return;
//     imu_prev = micros();

//     mpu.updateBias();
//     tilt_x = mpu.gx();
//     tilt_y = mpu.gy();
//     rot_z = mpu.gz();

//     // tilt_x = map(mpu.gx(), -1, 1, -90, 90);
//     // tilt_y = map(mpu.gy(), -1, 1, -90, 90);
//     // rot_z = map(mpu.gz(), -1, 1, -90, 90);
// }