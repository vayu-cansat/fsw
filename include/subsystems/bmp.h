#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include "variables.h"

float SEA_LEVEL_PRESSURE = 1013.25;
Adafruit_BMP280 bmp;

void bmp_setup()
{
#ifdef BMP_ACTIVE
    if (!bmp.begin(BMP280_ADDRESS_ALT))
    {
        Serial.println("❌ BMP");
    }
    else
    {
        Serial.println("✅ BMP Found!");
        bmp.setSampling(
            Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
            Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
            Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
            Adafruit_BMP280::FILTER_X16,      /* Filtering. */
            Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    }
#endif
}

void bmp_loop()
{
#ifdef BMP_ACTIVE
    bmp.takeForcedMeasurement();
    altitude = bmp.readAltitude(SEA_LEVEL_PRESSURE);
    pressure = bmp.readPressure();
#endif
}