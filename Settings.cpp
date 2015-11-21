/* Settings.cpp                  -*-C++-*- */

#include "Settings.h"

void Settings::init()
{
    m_tempUnits      = Fahrenheit;
    m_tempTargetLow  = 19;         // 19C, approximately 66F
    m_tempTargetHigh = 25;         // 25C, approximately 75F
    m_boostLow       = 1000.0;     // Boost off
    m_boostHigh      = -1000.0;    // Boost off 
    m_hvacState      = HvacOff;
}

float Settings::tempToCurrentUnits(float normalizedTemp) const
{
    if (Fahrenheit == m_tempUnits)
        return normalizedTemp * (9.0 / 5.0) + 32.0;
    else
        return normalizedTemp;
}

float Settings::tempToNormalForm(float tempFC) const
{
    if (Fahrenheit == m_tempUnits)
        return (tempFC - 32.0) * (5.0 / 9.0);
    else
        return tempFC;
}

Settings theCurrentSettings;
Settings theLastSettings;

/* end Settings.cpp */
