// ThermostatCore.h                  -*-C++-*- 
//
/// Look at the current temperature and decide whether to demand heat or cool.


#ifndef INCLUDED_THERMOSTATCORE_DOT_H
#define INCLUDED_THERMOSTATCORE_DOT_H
#include <application.h>

/// @brief Decide whether to demand heat or cool.
class ThermostatCore
{
    int          m_heatRelayPort;
    int          m_coolRelayPort;
	float        m_lowerTempC;
	float        m_upperTempC;
	bool         m_heatIsOn;
	bool         m_coolIsOn;
	uint32_t     m_timeOfLastChangeMs;
	
  public:
    // TBD: Constructor goes here
    
    /// @brief Set the lower and upper bounds of desired temperature range.
    /// @param lowerTempC Lower temperature bound in Celcius
    /// @param upperTempC Upper temperature bound in Celcius
    void setTargets(float lowerTempC, float upperTempC);

    void runThermostat(float currentTempC);
};

#endif // ! defined(INCLUDED_THERMOSTATCORE_DOT_H)
