// thermostat_main.cpp                  -*-C++-*-
//
/// @file Setup and main loop for smart thermostat

#include "Settings.h"
#include "TempSensor.h"
#include "Display.h"

#include <application.h>  // Arduino interface constants and functions

const int tempSensorPort = D1;
const int heatRelayPort  = D2;
const int coolRelayPort  = D3;
const int fanRelayPort   = D4;

/// @brief initialize the hardware and software modules
void setup()
{
    theCurrentSettings.initialize();
    theLastSettings = theCurrentSettings;
    theTempSensor.initialize(tempSensorPort);
    // theHvacControl.initialize(heatRelayPort, coolRelayPort, fanRelayPort);
    // theThermostatCore.initialize();
    theDisplay.initialize();
    // theUserInput.initialize();
    // theSchedule.initialize();
    // theBoostManager.initialize();
    // theInternetCommands.initialize();
    // theLogger.initialize();
}

/// @brief main loop called from the Photon OS
/// This function is called repeatedly, approximately once per milisecond,
/// from the Photo OS. Between calls to this function, the OS handles its WiFi
/// connection and other house keeping.  If this loop takes to long to
/// execute, the OS will drop the WiFi connection.  It is therefore important
/// to keep the amount of work minimal in each execution of the loop.  If
/// heavier processing is needed, the code should directly call the OS every
/// once in a while, or else call `delay`, which also gives the OS an
/// opportunity to do its processing.
void loop()
{
    theLastSettings = theCurrentSettings;
    // theThermostatCore.run();
    theDisplay.run();
    // theUserInput.run();
    // theSchedule.run();
    // theBoostManager.run();
    // theInternetCommands.run();
    // theLogger.run();
}

// End main.cpp
