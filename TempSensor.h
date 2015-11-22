/* TempSensor.h                  -*-C++-*- */

#ifndef INCLUDED_TEMPSENSOR_DOT_H
#define INCLUDED_TEMPSENSOR_DOT_H

/// @brief DHT11 or DHT22 temperature and humidity sensor
class TempSensor
{
    typedef unsigned      uint32_t;
    typedef unsigned char byte;

    int         m_port;          // Digital port used to communicate w/ sensor
    uint32_t    m_lastReadMs;    // Last time sensor was physically read (ms)
    float       m_relHumidity;   // Last read relative humidity in percent
    float       m_temperatureC;  // Last read temperature in degrees C

    /// @brief Physically read the sensor.
    /// @return 0 on success, non-zero on error
    /// Reads the DHT sensor and stores the temperature and humidity
    /// in `m_temperatureC` and `m_relHumidity`.
    int readSensor();

    void readTempAndHumidity();
    byte readByteFromSensor() const;

  public:
    explicit TempSensor(int port = -1);
    
    void initialize(int port);

    float getTemperature();   ///< Temperature in normalized form (degrees C)
    float getRelHumidity();   ///< Relative humidity in percent
};

extern TempSensor theTempSensor;  ///< Global temperature sensor

#endif // ! defined(INCLUDED_TEMPSENSOR_DOT_H)
