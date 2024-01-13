#pragma once
#include <Arduino.h>
#include "variables.h"
// #include "imu.h"
#include <XBee.h>

long int prev_millis;
int offset = 0;
bool rst = false;

#define SH 0x13a200
#define SL 0x41f450c4

XBee xbee = XBee();
XBeeAddress64 addr64 = XBeeAddress64(SH, SL);
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

String make_data_str()
{
    gps_time = (String)millis();
    optional_data = debug_message;
    String d = "" + team_id + "," + mission_time + "," + packet_counter + "," + mode + "," + state + "," + altitude + "," + air_speed + "," + hs_deployed + "," + pc_deployed + "," + temperature + "," + voltage + "," + pressure + "," + gps_time + "," + gps_altitude + "," + gps_latitude + "," + gps_longitude + "," + gps_stats + "," + tilt_x + "," + tilt_y + "," + rot_z + "," + cmd_echo + ",," + optional_data;
    return d;
}

void comms_setup()
{
    Serial.begin(9600);
    Serial.setTimeout(200);
    Serial1.begin(9600);
    xbee.setSerial(Serial1);
    delay(3000);
    // UPLOAD CODE
    // REMOVE RXTX
    // RESET XBEE
    // CONNECT RXTX
    // WORKS!
}

void start_comms()
{
    telemetry = true;
    prev_millis = millis();
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
        // imu_calibrate();
        debug_message = "IMU CALIBRATED";
        cmd_echo = cmd;
    }
}

void process(String cmd, String args)
{
    // Serial.println("Processing command: " + cmd + " " + args);
    // CMD,XXXX,CX,ON
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
    if (Serial.available() > 0)
    {
        cmd = Serial.readStringUntil('\n');
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
        if (telemetry)
        {
            packet_counter++;
            String dd = make_data_str();
            Serial.println(dd);

            uint8_t payload[dd.length()] = {};
            dd.getBytes(payload, dd.length());

            ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
            xbee.send(zbTx);

            // wait for tx status response
            xbee.readPacket();
            if (xbee.getResponse().isAvailable())
            {
                if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
                {
                    xbee.getResponse().getZBTxStatusResponse(txStatus);
                    if (txStatus.getDeliveryStatus() == SUCCESS)
                    {
                        // success
                        Serial.println("SUCCESS");
                    }
                    else
                    {
                        // error
                        Serial.println("ERROR");
                    }
                }
            }

            // debug_message = "";

            /// DEBUG INTERVALS ///
            // Serial.print(packet_counter);
            // Serial.print(" ");
            // Serial.print(interval);
            // Serial.print(" ");
            // Serial.print(offset);
            // Serial.print(" ");
            // Serial.println(millis());
        }
        prev_millis = millis();
        offset = prev_millis % rate;
    }
}