/* TempSensor.cpp                  -*-C++-*- */

#include "TempSensor.h"

#include <application.h>  // Arduino interface constants and functions

// Choose between DHT22 and DHT11 temperature & humidity sensor
#define DHT22 22  // DHT22 is the higher-accuracy sensor (16-bit measurements)
#define DHT11 11  // DHT11 is the lower-accuracy sensor (8-bit measurements)
#define DHT_MODEL 22

TempSensor theTempSensor;

// Wait at least 2 seconds (2000ms) between physical reads of the sensor.
const unsigned minTimeBetweenReadingsMs = 2000;

TempSensor::TempSensor(int port)
    : m_port(port)
{
    if (port > 0)
        initialize(port);
}

void TempSensor::initialize(int port)
{
    m_port = port;
    m_lastReadMs = 0;         // Read temperature ASAP
    m_relHumidity = 0;
    m_temperatureC = 21.0;    // Reasonable temperature, just in case

    pinMode(m_port, INPUT_PULLUP); 
}

float TempSensor::getTemperature()
{
    readTempAndHumidity();
    return m_temperatureC;
}

float TempSensor::getRelHumidity()
{
    readTempAndHumidity();
    return m_relHumidity;
}

void TempSensor::readTempAndHumidity()
{
    // Don't read the temperature sensor too often -- it will return
    // bad results and/or damage the sensor
    millis_t currentTimeMs = millis();
    if (currentTimeMs < minTimeBetweenReadingsMs)
    {
        // Reading temperature too soon. Wait for minimum time to elapse.
        delay(minTimeBetweenReadingsMs - currentTimeMs);
        currentTimeMs = millis();
    }
    
    if (currentTimeMs < m_lastReadMs) 
        m_lastReadMs = 0;  // Timer has wrapped around

    if (currentTimeMs - m_lastReadMs < minTimeBetweenReadingsMs)
        return;  // Not enough time has elapsed since last physical read

    // Reset timer. If an error occurs, we still want to wait before trying
    // again.
    m_lastReadMs = currentTimeMs;
    
    // Read the sensor. Ignore error -- values will be unchanged on error.
    int err = readSensor();
    if (err)
        m_relHumidity = err;

    // Reset port
    pinMode(m_port, INPUT_PULLUP); 
}

int TempSensor::readSensor()
{
    // Bytes being read fron DHTxx sensor
    byte dhtBytes[4];

    // Start protocol: Temporarily set pin to OUTPUT mode and pull low for
    // 20ms.
    pinMode(m_port, OUTPUT); 
    digitalWrite(m_port, LOW); 
    delay(20); 
    pinMode(m_port, INPUT_PULLUP); 

    // Let float HIGH for 40us
    delayMicroseconds(40); 

    // Expect input to go low immediately for at MOST 80us, then high
    // for at LEAST 80us
    if (digitalRead(m_port))
        return 1;  // ERROR.  Try again later
    delayMicroseconds(80); 
    if (! digitalRead(m_port))
        return 2;  // ERROR.  Try again later
    delayMicroseconds(80);

    // Read 4 bytes
    byte checksum = 0;
    for (int i = 0; i < 4; ++i)
    {
        dhtBytes[i] = readByteFromSensor();
        checksum += dhtBytes[i];
    }

    // Read checksum byte and compare to computed checksum
    if (readByteFromSensor() != checksum)
        return 3;  // ERROR.  Try again later

#if DHT_MODEL == DHT11
    m_relHumidity  =  dhtBytes[0];       // Humidity stored in single byte
    m_temperatureC =  dhtBytes[2];       // Temperature stored in single byte
#else // end else DHT22
    unsigned short value16;

    // Compute 16-bit humidity in tenths of a percent
    value16 = (dhtBytes[0] << 8) + dhtBytes[1];
    m_relHumidity = value16 * 0.1;

    // Compute 16-bit temperature in tenths of a degree C
    value16 = (dhtBytes[2] << 8) + dhtBytes[3];
    
    // Temperature is in signed-magnitude format; if high bit is set
    // value is negative.
    if (value16 & 0x1000)
        m_temperatureC = -0.1 * (value16 & 0x7fff);
    else
        m_temperatureC = 0.1 * value16;
#endif // end else DHT22

    return 0; // Success
}

byte TempSensor::readByteFromSensor() const
{
    byte result = 0;

    // Read 8 bits
    for (int i = 0; i < 8; ++i) 
    { 
        // Wait for pin to go high
        while(digitalRead(m_port) == LOW)
            ;

        // Start of high pulse.  If high pulse is over 30us, then this
        // represents a high bit; otherwise, it represents a low bit.
        delayMicroseconds(30); 
        result <<= 1;
        if (digitalRead(m_port) == HIGH) 
            result |= 1;  // Over 30us: high bit
        // Else low bit, nothing is added to the result

        // Wait for pin to go low again
        while (digitalRead(m_port) == HIGH)
            ;
    } 

    return result; 
}

/* End TempSensor.cpp */
