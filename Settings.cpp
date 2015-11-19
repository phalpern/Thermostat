/* Settings.cpp                  -*-C++-*- */

#include "Settings.h"

void Settings::init()
{
    m_tempScale      = Fahrenheit;
    m_tempTargetLow  = 19;         // 19C, approximately 66F
    m_tempTargetHigh = 25;         // 25C, approximately 75F
    m_boostLow       = -1.0;       // Boost off
    m_boostHigh      = -1.0;       // Boost off 
    m_hvacState      = HvacOff;
}

Settings theCurrentSettings;
Settings theLastSettings;

/* end Settings.cpp */
