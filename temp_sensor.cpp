/* temp_sensor.cpp                  -*-C++-*- */

#include "temp_sensor.h"

TempSensor::TempSensor(int port)
    : m_port(port), m_lastChecked(0)
{
}

/* End temp_sensor.cpp */
