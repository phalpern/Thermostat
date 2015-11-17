/* Settings.h                  -*-C++-*- */

#ifndef INCLUDED_SETTINGS_DOT_H
#define INCLUDED_SETTINGS_DOT_H

#include <application.h>  // Arduino functions and macros

/// @brief TempScale is used to select whether to read temp in C or F
enum TempScale_t { Celcius, Fahrenheit };

/// @brief State of the HVAC system (what is actually happening in basement)
enum HvacState_t { HvacOff, HvacHeat, HvacCool, HvacFan };

/// @brief Operation mode of the thermostat
enum Mode_t { ModeOff, ModeNormal, ModeOverride, ModeBoost, ModeAway };

struct Settings
/// @brief Temperature and mode settings affecting entire thermostat
{
    TempScale_t m_tempScale;
    float       m_tempTargetLow;
    float       m_tempTargetHigh;
    float       m_boostLow;
    float       m_boostHigh;
    HvacState_t m_hvacState;
};

#endif // ! defined(INCLUDED_SETTINGS_DOT_H)