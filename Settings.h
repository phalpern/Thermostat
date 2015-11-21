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
    TempUnits_t m_tempUnits;      ///< Celsius or Fahrenheit
    float       m_tempPrecision;  ///< Human-readable precision for temperature
    float       m_tempTargetLow;  ///< Temperature threshold for heat
    float       m_tempTargetHigh; ///< Temperature threshold for cooling
    float       m_boostLow;       ///< Temporary temperature threshold for heat
    float       m_boostHigh;      ///< Temporary temperature threshold for cool
    HvacState_t m_hvacState;      ///< Current state of heat/cool hardware

    /// @brief Initialize to default settings
    void initialize();  

    /// @brief  Convert a temperature to the current temperature scale
    /// @param  normalizedTemp Temperature in normalized form (degrees C)
    /// @param  round          true if results should be rounded for humans
    /// @return Temperature in either F or C, depending on m_tempUnits.
    float tempToCurrentUnitsFC(float normalizedTemp, bool round = true) const;

    /// @brief  Convert a temperature to normalized form (degrees C)
    /// @param  tempFC Temperature in either F or C, depending on m_tempUnits.
    /// @return Temperature in normalized form (degrees C)
    float tempToNormalForm(float tempFC) const;
};

/// @brief Current settings
/// The current settings must be initialized before any other module.
extern Settings theCurrentSettings;

/// @brief Settings last time through main loop
/// This variable holds the settings that were in effect the previous time
/// through the main loop. A function can compare individual settings between
/// `theCurrentSettings` and `theLastSettings` to see if anything has changed.
extern Settings theLastSettings;

#endif // ! defined(INCLUDED_SETTINGS_DOT_H)
