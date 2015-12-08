/* Display.cpp                  -*-C++-*- */

#include "Display.h"
#include "TempSensor.h"
#include "Settings.h"

#include <application.h>
#include <string.h>

Display theDisplay;

#if STUB_DISPLAY
// Display template string
const char displayTemplate[] =
    "/ Current: 00.0C xx%  Settings: min 00.0C max 00.0C  HVAC: Heat\r";

// Offsets into template of various fields
const int posTickTock = 0;
const int posCurrTemp = 11;
const int posCurrHumidity = 17;
const int posLowSetting = 36;
const int posHighSetting = 46;
#endif

// Format the (normalized) temperature into a 5-byte (with no null
// terminator), buffer using the current settings.  Return true.
static bool formatTemp(char buffer[5], float temp)
{
    // Show one digit after the decimal point if precision is less than 1
    bool showDecimal = (theCurrentSettings.m_tempPrecision < 1.0);

    float tempFC = theCurrentSettings.tempToCurrentUnitsFC(temp);
    // Get integer portion of temperature.  If showing fractions, round off.
    int tempInt = showDecimal ? int(tempFC) : int(tempFC + 0.5);
    buffer[0] = tempInt > 9 ? (tempInt / 10 + '0') : ' ';
    buffer[1] = tempInt % 10 + '0';

    if (showDecimal)
    {
        // Precision better than 1.0 degree.  Display one decimal place.
        buffer[2] = '.';
        buffer[3] = int(tempFC * 10) % 10 + '0';
    }
    else
        buffer[2] = buffer[3] = ' ';

    buffer[4] = theCurrentSettings.m_tempUnits == Fahrenheit ? 'F' : 'C';

    return true;
}

// Format the relative humidity into a 3-byte (with no null terminator).
// Return true.
static bool formatHumidity(char buffer[3], float humidity)
{
    int humidityInt = int(humidity + 0.5); // Round to integer
    buffer[0] = humidityInt > 9 ? (humidityInt / 10 + '0') : ' ';
    buffer[1] = humidityInt % 10 + '0';
    buffer[2] = '%';

    return true;
}

void Display::initialize()
{
    m_lastTemp = 0.0;
    m_lastHumidity = -1.0;

#if STUB_DISPLAY
    Serial.begin(9600);
    strcpy(m_buffer, displayTemplate);
    delay(300);
    Serial.println("Welcome to the Thermostat project!\n");
    formatTemp(m_buffer + posLowSetting,
               theCurrentSettings.m_tempTargetLow);
    formatTemp(m_buffer + posHighSetting,
               theCurrentSettings.m_tempTargetHigh);
#else
#endif
}

void Display::run()
{
    bool updated = false;  // Set to true if something changed

#if STUB_DISPLAY
    unsigned long tick = millis();
    char tickTock = "/-\\|"[(tick / 1000) % 4];
    if (tickTock != m_buffer[posTickTock]) 
    {
        m_buffer[posTickTock] = tickTock;
        updated = true;
    }

    float temp = theTempSensor.getTemperature();
    if (temp != m_lastTemp) 
        updated = formatTemp(m_buffer + posCurrTemp, temp);
    m_lastTemp = temp;

    float humidity = theTempSensor.getRelHumidity();
    if (humidity != m_lastHumidity)
        updated = formatHumidity(m_buffer + posCurrHumidity, humidity);
    m_lastHumidity = humidity;

    if (theCurrentSettings.m_tempTargetLow != theLastSettings.m_tempTargetLow)
        updated = formatTemp(m_buffer + posLowSetting,
                             theCurrentSettings.m_tempTargetLow);

    if (theCurrentSettings.m_tempTargetHigh != theLastSettings.m_tempTargetHigh)
        updated = formatTemp(m_buffer + posHighSetting,
                             theCurrentSettings.m_tempTargetHigh);

    if (theCurrentSettings.m_tempUnits != theLastSettings.m_tempUnits)
    {
        // Switch between C and F requires reformatting all temperatures.
        formatTemp(m_buffer + posCurrTemp, temp);
        formatTemp(m_buffer + posLowSetting,
                   theCurrentSettings.m_tempTargetLow);
        formatTemp(m_buffer + posHighSetting,
                   theCurrentSettings.m_tempTargetHigh);
        updated = true;
    }

    if (updated)
        Serial.print(m_buffer);
#endif
}

/* End Display.cpp */
