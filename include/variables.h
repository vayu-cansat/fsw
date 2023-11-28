#pragma once
#include <Arduino.h>

// OUTPUT FORMAT
// TEAM_ID, MISSION_TIME, PACKET_COUNT, MODE, STATE, ALTITUDE, AIR_SPEED, HS_DEPLOYED, PC_DEPLOYED, TEMPERATURE, VOLTAGE, PRESSURE, GPS_TIME, GPS_ALTITUDE, GPS_LATITUDE, GPS_LONGITUDE, GPS_SATS, TILT_X, TILT_Y, ROT_Z, CMD_ECHO

///// VARIABLES /////
bool debugging = false;
bool telemetry = false;
int rate = 1000;

bool sim_enabled = false;
bool sim_active = false;
/////////////////////

String cmd = "";
String debug_message = "";
String optional_data = "";

///// FLIGHT VARIABLES /////
String team_id = "2117";
long int packet_counter = 0;
String mode = "S";            // S-sim, F-flight
String state = "LAUNCH_WAIT"; // Team decides
String cmd_echo = "";
// switches
String hs_deployed = "N"; // P - heat shield deployed, N-no
String pc_deployed = "N"; // C - Parachute deployed (100m), N-no
// time
String mission_time = "13:14:02";
String gps_time = "13:14:03";
// sensor readings
float altitude = 0.0;
float air_speed = 0.0;
float temperature = 0.0;
float pressure = 0.0;
float voltage = 0.0;
float gps_altitude = 0.0;
double gps_latitude = 0.0;
double gps_longitude = 0.0;
int gps_stats = 0; // number of sats
float tilt_x = 0.0;
float tilt_y = 0.0;
float rot_z = 0.0;
////////////////////////////