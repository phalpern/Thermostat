// ThermostatCore.cpp                  -*-C++-*-

#include "ThermostatCore.h"

/* tempFuzz is the amount in degrees C the over/under the lower/upper temp bounds that the thermostat allows
 * the temperature in the house to go before it turns on/off the heat/airconditioning
 */
const float tempFuzzC = 0.3; 


void ThermostatCore::setTargets(float lowerTempC, float upperTempC)
{
	m_lowerTempC = lowerTempC;
	m_upperTempC = upperTempC;
}

void ThermostatCore::runThermostat(float currentTempC)
{
	bool heatShouldBeOn = m_heatIsOn;
	bool coolShouldBeOn = m_coolIsOn;
	const int minimumChangeInterval = 6000;

	if (m_lowerTempC > m_upperTempC-6*tempFuzzC) 
	{
		return; //safety feature to keep the upper and lower limits from getting too close together
	} 
	
	
	if (currentTempC < m_upperTempC - tempFuzzC) //if the air conditioning has cooled down the house enough
	{
		coolShouldBeOn = false;
	}
	
	if (currentTempC > m_lowerTempC + tempFuzzC) //if the heat has warmed up the house enough
	{
		heatShouldBeOn = false;
	}

	if (currentTempC > m_upperTempC + tempFuzzC) //if it gets to hot
	{
		coolShouldBeOn = true;
	}

	if (currentTempC < m_lowerTempC - tempFuzzC) //if it gets to cold
	{
		heatShouldBeOn = true;
	}
	

	if (m_heatIsOn != heatShouldBeOn ||
		m_coolIsOn != coolShouldBeOn)
	{
	
		// this code happens only if something has changed.
		uint32_t currentTimeMs = millis();

		if (currentTimeMs < m_timeOfLastChangeMs)
		{
			m_timeOfLastChangeMs = 0; //timer has wrapped around
		}
		if (currentTimeMs - m_timeOfLastChangeMs > minimumChangeInterval) // has enough time passed since we last changed something?
		{
			// it is safe to turn cool off because it is either already off or is about to be turned off
			// by turning the cool off we eliminate having the heat and cool on at the same time
			digitalWrite(m_coolRelayPort, LOW);
			digitalWrite(m_heatRelayPort, heatShouldBeOn);
			digitalWrite(m_coolRelayPort, coolShouldBeOn);

			m_heatIsOn = heatShouldBeOn;
			m_coolIsOn = coolShouldBeOn;

			m_timeOfLastChangeMs = currentTimeMs;
		}
		else //not enough time has passed since we last changed something
		{
			return;
		}
		
	}

}

/* End ThermostatCore.cpp */
