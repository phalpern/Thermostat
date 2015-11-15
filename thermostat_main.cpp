// thermostat_main.cpp                  -*-C++-*-
//
/// @file Setup and main loop for smart thermostat

#include "globals.h"
#include "temp_sensor.h"

const int tempSensorPort = D0;
const int heatRelayPort = D1;
const int coolRelayPort = D2;
const int fanRelayPort = D3;

TempSensor theTempSensor;
ThermostatCore theThermostatCore;

void setup()
{
    theTempSensor.initialize(tempSensorPort);
    // theThermostatCore.initialize(heatRelayPort, coolRelayPort, fanRelayPort);
}

// @brief calculate the target thermostat temperatures
// @inputs g_lowerTempSetC, g_upperTempSetC, g_lowerTempBoostC,
// g_upperTempBoostC
// @outputs g_lowerTempTargetC, g_upperTempTargetC
// Computes the target temperature range based on the schedule, manual
// settings, and boost settings.  Boost settings take precidence, followed by
// the normal global settings.  The global settings are set by the
// stored schedule but can be overridden manually or by the home/not home
// detection system.
void calcTargetTemp()
{
    static const float 

    if (g_lowerTempBoostC > 0.0)
    {
        g_lowerTempTargetC = g_lowerTempBoostC;
    }
}

void loop()
{
    float currentTempC = theTempSensor.getTemperature();
    g_currentTempCF = theTempSensor.getTemperature(g_tempScale);
    g_currentHumidity = theTempSensor.getRelHumidity();

    float lowerTargetTempC, upperTargetTempC;
    calcTargetTemps(&lowerTargetTempC, &upperTargetTempC);
    theThermostatCore.setTargets(lowerTargetTempC, upperTargetTempC);
    theThermostatCore.runThermostat(currentTempC);
}

// End main.cpp
