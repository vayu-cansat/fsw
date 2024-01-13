#include <Arduino.h>
#include "variables.h"
#include "comms.h"
#include "subsystems.h"
#include "imu.h"

void setup()
{
  comms_setup();

#ifdef IMU_ACTIVE
  // imu_setup();
#endif
  start_comms();
}

void loop()
{

#ifdef IMU_ACTIVE
  // imu_loop();
#endif

  comms_loop();
}
