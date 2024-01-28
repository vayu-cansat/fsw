#include <Arduino.h>
#include <Adafruit_GPS.h>

Adafruit_GPS GPS(&Serial2);

#define GPSECHO false
unsigned long gps_timer = millis();
int update_freq = 1000;

String format_coords(double coord)
{
    String c = "";
    int deg = (int)coord / 100;
    c += (String)deg + "° ";
    double min = coord - (deg * 100);
    c += (String)min + "'";
    return c;
}

void gps_setup()
{
#ifdef GPS_ACTIVE
    Serial2.begin(9600);
    GPS.begin(9600);
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // Update frequency
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    // Request updates on antenna status, comment out to keep quiet
    GPS.sendCommand(PGCMD_ANTENNA);
#endif
}

void gps_loop()
{
#ifdef GPS_ACTIVE
    char c = GPS.read();
    if ((c) && (GPSECHO))
        Serial.write(c);

    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived())
    {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
        // Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return;                     // we can fail to parse a sentence in which case we should just wait for another
    }

    if ((millis() - gps_timer) > (unsigned long)update_freq)
    {
        gps_timer = millis();
        if (GPS.fix)
        {
            gps_sats = (int)GPS.satellites;
            gps_altitude = GPS.altitude;
            // gps_latitude = format_coords(GPS.latitude) + GPS.lat;
            // gps_longitude = format_coords(GPS.longitude) + GPS.lon;
            gps_latitude = (String)GPS.latitudeDegrees;
            gps_longitude = (String)GPS.longitudeDegrees;
            gps_time = "";
            if (GPS.hour < 10)
            {
                gps_time += "0";
            }
            gps_time += (String)GPS.hour + ":";
            if (GPS.minute < 10)
            {
                gps_time += "0";
            }
            gps_time += (String)GPS.minute + ":";
            if (GPS.seconds < 10)
            {
                gps_time += "0";
            }
            gps_time += (String)GPS.seconds;
        }
        else
        {
            gps_sats = 0;
            gps_altitude = 0;
            gps_latitude = "0.0";
            gps_longitude = "0.0";
            gps_time = "00:00:00";
        }
    }
#endif
}