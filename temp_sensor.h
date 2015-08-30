/* temp_sensor.h                  -*-C++-*- */

#ifndef INCLUDED_TEMP_SENSOR_DOT_H
#define INCLUDED_TEMP_SENSOR_DOT_H

#include "globals.h"

/// @brief DHT11 or DHT22 temperature and humidity sensor
class TempSensor
{
    int         m_port;
    int         m_lastChecked;
    float       m_temperature;
    float       m_relHumidity;

  public:
    TempSensor(int port);

    float getTemperature(TempScale scale = g_temp_scale);
    float getRelHumidity();
};

#endif // ! defined(INCLUDED_TEMP_SENSOR_DOT_H)
