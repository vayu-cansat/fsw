#pragma once
#include <Arduino.h>
#include "variables.h"
#include "subsystems/imu.h"

long int prev_millis;
int offset = 0;
bool rst = false;

String make_data_str()
{
    optional_data = debug_message;
    String d = "" + team_id + "," + mission_time + "," + packet_counter + "," + mode + "," + state + "," + altitude + "," + air_speed + "," + hs_deployed + "," + pc_deployed + "," + temperature + "," + voltage + "," + pressure + "," + gps_time + "," + gps_altitude + "," + gps_latitude + "," + gps_longitude + "," + gps_sats + "," + tilt_x + "," + tilt_y + "," + rot_z + "," + cmd_echo + ",," + optional_data;
    return d;
}

String make_debug_str()
{
    String d = "";
    d += "TEAM    : " + team_id + "\n";
    d += "MTime   : " + mission_time + "\n";
    d += "PCOUNT  : " + (String)packet_counter + "\n";
    d += "MODE    : " + mode + "\n";
    d += "STATE   : " + state + "\n";
    d += "ALTI    : " + (String)altitude + "\n";
    d += "AIR SPD : " + (String)air_speed + "\n";
    d += "HS DEPL : " + hs_deployed + "\n";
    d += "PC DEPL : " + pc_deployed + "\n";
    d += "TEMP    : " + (String)temperature + "\n";
    d += "VOLT    : " + (String)voltage + "\n";
    d += "PRSS    : " + (String)pressure + "\n";
    d += "GPS TIME: " + gps_time + "\n";
    d += "GPS ALTI: " + (String)gps_altitude + "\n";
    d += "GPS LATI: " + (String)gps_latitude + "\n";
    d += "GPS LONG: " + (String)gps_longitude + "\n";
    d += "GPS SATS: " + (String)gps_sats + "\n";
    d += "TILTX   : " + (String)tilt_x + "\n";
    d += "TILTY   : " + (String)tilt_y + "\n";
    d += "ROTZ    : " + (String)rot_z + "\n";
    d += "CMD     : " + cmd_echo + "\n";
    d += "OPT DATA: " + optional_data + "\n";
    return d;
}

void start_comms()
{
    telemetry = true;
    prev_millis = millis();
}

void comms_setup()
{
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    gpio_put(25, 1);
    delay(250);
    gpio_put(25, 0);

    Serial.begin(9600);
    Serial.setTimeout(200);
    delay(10000);
    Serial1.begin(9600);
    Serial1.setTimeout(200);

    // DISCONNECT BOTH RXTX PINS FROM XBEE
    // RESET BOTH XBEE
    // WAIT FOR GCS XBEE RSSI GREEN LED
    // CONNECT GCS RXTX
    // CONNECT CANSAT RXTX
    // BOOM! TELEMETRY WORKING
}

void process(String cmd)
{
    // Serial.println("Processing command: " + cmd);
    if (cmd == "ACK")
    {
        debug_message = "";
        // cmd_echo = cmd;
    }
    if (cmd == "CAL")
    {
        imu_calibrate();
        debug_message = "IMU CALIBRATED";
        cmd_echo = cmd;
    }
}

void process(String cmd, String args)
{
    // Serial.println("Processing command: " + cmd + " " + args);
    // CMD,2117,CX,OFF
    if (cmd == "CX")
    {
        if (args == "ON")
        {
            telemetry = true;
            cmd_echo = cmd + args;
            debug_message = "TELEMETRY ON";
        }
        else if (args == "OFF")
        {
            telemetry = false;
            cmd_echo = cmd + args;
            debug_message = "TELEMETRY OFF";
        }
    }
    else if (cmd == "ST")
    {
        if (args == "GPS")
        {
            // set time from GPS
            cmd_echo = cmd + args;
            debug_message = "GPS TIME SET";
        }
        else
        {
            // validate time is in correct format
            if (args.length() == 8)
            {
                mission_time = args;
                cmd_echo = cmd + args;
                debug_message = "MISSION TIME SET MANUALLY";
            }
        }
    }
    else if (cmd == "SIM")
    {
        if (args == "ENABLE")
        {
            sim_enabled = true;
            cmd_echo = cmd + args;
            debug_message = "SIMULATION ENABLED [1]";
        }
        else if (args == "ACTIVATE")
        {
            if (sim_enabled)
            {
                sim_active = true;
                cmd_echo = cmd + args;
                debug_message = "SIMULATION ACTIVATED [2]";
            }
        }
        else if (args == "DISABLE")
        {
            sim_enabled = false;
            sim_active = false;
            cmd_echo = cmd + args;
            debug_message = "SIMULATION DISABLED";
        }
    }
    else if (cmd == "SIMP")
    {
        // sea level approx = 1013.25
        if (sim_active)
        {
            // set pressure
            pressure = args.toFloat();
            cmd_echo = cmd + args;
            debug_message = "PRESSURE SET";
        }
        else
        {
            debug_message = "SIMULATION INACTIVE";
        }
    }
    else if (cmd == "BCN")
    {
        if (args == "ON")
        {
            // turn on beacon
            cmd_echo = cmd + args;
            debug_message = "BEACON ON";
        }
        else if (args == "OFF")
        {
            // turn off beacon
            cmd_echo = cmd + args;
            debug_message = "BEACON OFF";
        }
    }
}

void comms_loop()
{
    // check for incoming commands
    // Incoming CMD format: CMD,XXXX,CMD_NAME,ARGS
    if (Serial1.available() > 0)
    {
        cmd = Serial1.readStringUntil('\n');
        if (cmd.startsWith("CMD"))
        {
            cmd = cmd.substring(9, cmd.length()); // remove CMD,XXXX,
            String cmd_name = "", args = "";
            cmd_name = cmd.substring(0, cmd.indexOf(",")); // CMD_NAME
            int has_args = cmd.indexOf(",");
            if (has_args == -1)
            {
                process(cmd_name);
                cmd = "";
            }
            else
            {
                cmd = cmd.substring(has_args + 1); // ARGS
                String args = cmd;
                process(cmd_name, args);
                cmd = "";
            }
        }
    }

    // telemetry every second
    int interval = millis() - prev_millis;
    if (interval > (rate - abs(offset)))
    {
        String dd = make_data_str() + "\n\r";
        Serial.print(dd);
        // Serial.println(make_debug_str());
#ifdef COM_ACTIVE
        if (telemetry)
        {
            gpio_put(25, 1);
            packet_counter++;
            Serial1.print(dd);
            gpio_put(25, 0);
        }
#endif
        prev_millis = millis();
        offset = prev_millis % rate;
    }
}