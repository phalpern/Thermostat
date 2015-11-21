/* Settings.h                  -*-C++-*- */

#ifndef INCLUDED_SETTINGS_DOT_H
#define INCLUDED_SETTINGS_DOT_H

/// @brief TempScale is used to select whether to read temp in C or F
enum TempUnits_t { Celcius, Fahrenheit };

/// @brief State of the HVAC system (what is actually happening in basement)
enum HvacState_t { HvacOff, HvacHeat, HvacCool, HvacFan };

/// @brief Operation mode of the thermostat
enum Mode_t { ModeOff, ModeNormal, ModeOverride, ModeBoost, ModeAway };

/// @brief Temperature and mode settings affecting entire thermostat
struct Settings
{
    TempUnits_t m_tempUnits;
    float       m_tempTargetLow;
    float       m_tempTargetHigh;
    float       m_boostLow;
    float       m_boostHigh;
    HvacState_t m_hvacState;

    /// @brief Initialize to default settings
    void init();  

    /// @brief  Convert a temperature to the current temperature scale
    /// @param  normalizedTemp Temperature in normalized form (degrees C)
    /// @return Temperature in either F or C, depending on m_tempUnits.
    float tempToCurrentUnits(float normalizedTemp) const;

    /// @brief  Convert a temperature to normalized form (degrees C)
    /// @param  tempFC Temperature in either F or C, depending on m_tempUnits.
    /// @return Temperature in normalized form (degrees C)
    float tempToNormalForm(float tempFC) const;
};

extern Settings theCurrentSettings;  ///< Current settings
extern Settings theLastSettings;     ///< Settings last time through main loop

#endif // ! defined(INCLUDED_SETTINGS_DOT_H)
