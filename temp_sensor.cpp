/* temp_sensor.cpp                  -*-C++-*- */

#include "temp_sensor.h"

// Wait at least 2 seconds (2000ms) between physical reads of the sensor.
const unsigned minTimeBetweenReadingsMs = 2000;

TempSensor::TempSensor(int port)
    : m_port(port), m_lastCheckedMs(-1)
{
    if (port > 0)
        initialize(port);
}

void TempSensor::initialize(int port)
{
    m_port = port;
    m_lastReadMs = millis() - minTimeBetweenReadingsMs;

    pinMode(m_port, OUTPUT); 
    digitalWrite(m_port, HIGH); 
}

float TempSensor::getTemperature(TempScale scale = Celcius)
{
    readTempAndHumidity();
    if (scale == Fahrenheit)
        return cToF(m_temperatureC);
    else
        return m_temperatureC;
}

float TempSensor::getRelHumidity()
{
    readTempAndHumidity();
    return m_relHumidity;
}

float TempSensor::cToF(float tempC)
{
    return tempC * 9 / 5 + 32;
}

void TempSensor::readTempAndHumidity()
{
    // Don't read the temperature sensor too often -- it will return
    // bad results and/or damage the sensor
    uint32_t currentTimeMs = millis();
    if (currentTimeMs < m_lastReadMs) 
        m_lastReadMs = 0;  // Timer has wrapped around

    if (currentTimeMs - m_lastReadMs < minTimeBetweenReadingsMs)
        return;  // Not enough time has elapsed since last physical read

    // Reset timer. If an error occurs, we still want to wait before trying
    // again.
    m_lastReadMs = currentTimeMs;
    
    if (readSensor() != 0)
        delay(7);  // If error, pause before resetting the port.

    // Reset port
    pinMode(m_port,OUTPUT); 
    digitalWrite(m_port,HIGH); 
}

int TempSensor::readSensor()
{
    // Bytes being read fron DHTxx sensor
    byte dhtBytes[4];

    // Start protocol: Pull pin low for 20ms, then high for 40us,
    // then set to INPUT mode and start listening.
    digitalWrite(m_port, LOW); 
    delay(20); 
    digitalWrite(m_port, HIGH); 
    delayMicroseconds(40); 
    pinMode(m_port, INPUT); 
    //delayMicroseconds(40); 

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
    for (int i = 0; i < 5; ++i)
    {
        dhtBytes[i] = readByteFromSensor();
        checksum += dhtBytes[i];
    }

    // Read checksum byte and compare to computed checksum
    if (readByteFromSensor() != checksum)
        return 3;  // ERROR.  Try again later

    m_relHumidity  =  dhtBytes[0];       // Integral part of humidity
    m_relHumidity  += dhtBytes[1] * 0.1; // Tenths of a percent humidity
    m_temperatureC =  dhtBytes[2];       // Integral part of temperature C
    m_temperatureC += dhtBytes[3] * 0.1; // Tenths degrees C

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
        if (digitalRead(m_port) == HIGH) 
            result |= (1 << bit);  // Over 30us: high bit
        // Else low bit, nothing is added to the result

        // Wait for pin to go low again
        while (digitalRead(m_port) == HIGH)
            ;
    } 

    return result; 
}

/* End temp_sensor.cpp */
