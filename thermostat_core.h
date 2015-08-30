// thermostat_core.h                  -*-C++-*- 
//
/// Look at the current temperature and decide whether to demand heat or cool.


#ifndef INCLUDED_THERMOSTAT_CORE_DOT_H
#define INCLUDED_THERMOSTAT_CORE_DOT_H

/// @brief Decide whether to demand heat or cool.
class ThermostatCore
{
    int m_heatRelay;
    int m_coolRelay;

  public:
    // TBD: Constructor goes here
    
    /// @brief Set the lower and upper bounds of desired temperature range.
    /// @param lowerTempC Lower temperature bound in Celcius
    /// @param upperTempC Upper temperature bound in Celcius
    void setTargets(float lowerTempC, float upperTempC);

    void runThermostat(float currentTempC);
}

#endif // ! defined(INCLUDED_THERMOSTAT_CORE_DOT_H)
