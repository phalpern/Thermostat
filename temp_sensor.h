/* temp_sensor.h                  -*-C++-*- */

#ifndef INCLUDED_TEMP_SENSOR_DOT_H
#define INCLUDED_TEMP_SENSOR_DOT_H

#include "globals.h"

/// @brief DHT11 or DHT22 temperature and humidity sensor
class TempSensor
{
    int         m_port;          // Digital port used to communicate w/ sensor
    uint32_t    m_lastReadMs;    // Last time sensor was physically read (ms)
    float       m_relHumidity;   // Last read relative humidity in percent
    float       m_temperatureC;  // Last read temperature in degrees C

    void readTempAndHumidity();

    /// @brief Physically read the sensor.
    /// @return 0 on success, non-zero on error
    /// Reads the DHT sensor and stores the temperature and humidity
    /// in `m_temperatureC` and `m_relHumidity`.
    int readSensor();

    byte readByteFromSensor() const;

  public:
    explicit TempSensor(int port = -1);
    
    void initialize(int port);

    float getTemperature(TempScale scale = Celcius);
    float getRelHumidity();

    static float cToF(float tempC);
};

#endif // ! defined(INCLUDED_TEMP_SENSOR_DOT_H)
