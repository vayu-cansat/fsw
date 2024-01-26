#include <Arduino.h>
#include "variables.h"
#include "comms.h"
#include "subsystems.h"
#include "rtc.h"
#include "imu.h"

void setup()
{
  comms_setup();

#ifdef IMU_ACTIVE
  // imu_setup();
#endif
  start_comms();
  rtc_setup();
}

void loop()
{

#ifdef IMU_ACTIVE
  // imu_loop();
#endif
  rtc_loop();
  comms_loop();
}
