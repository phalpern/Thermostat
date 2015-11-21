/* Settings.cpp                  -*-C++-*- */

#include "Settings.h"

void Settings::initialize()
{
    m_tempUnits      = Fahrenheit; // Default temperture unit is degrees F
    m_tempPrecision  = 0.5;        // Show temperature to precision of 0.5F
    m_tempTargetLow  = 15.5;       // 15.5C, approximately 60F
    m_tempTargetHigh = 25.5;       // 26.5C, approximately 80F
    m_boostLow       = -1000.0;    // Boost off
    m_boostHigh      = 1000.0;     // Boost off 
    m_hvacState      = HvacOff;
}

float Settings::tempToCurrentUnitsFC(float normalizedTemp, bool round) const
{
    double result;
    if (Fahrenheit == m_tempUnits)
        result = normalizedTemp * (9.0 / 5.0) + 32.0;
    else
        result = normalizedTemp;

    if (round) 
    {
        double roundTo = m_tempPrecision; // computations in double precision
        result = int(result / roundTo + roundTo) * roundTo;
    }

    return float(result);  // Truncate to single-precision float
}

float Settings::tempToNormalForm(float tempFC) const
{
    if (Fahrenheit == m_tempUnits)
        return float((tempFC - 32.0) * (5.0 / 9.0));
    else
        return tempFC;
}

Settings theCurrentSettings;
Settings theLastSettings;

/* end Settings.cpp */
