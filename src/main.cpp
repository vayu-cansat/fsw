#include <Arduino.h>
#include "variables.h"
#include "subsystems.h"
#include <Wire.h>

void setup()
{
  Wire.begin();
  comms_setup();
  bmp_setup();
  imu_setup();
  rtc_setup();
  gps_setup();
  start_comms();
}

void loop()
{
  imu_loop();
  rtc_loop();
  gps_loop();
  bmp_loop();
  comms_loop();
}
