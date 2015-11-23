/* Display.h                  -*-C++-*-  */

#ifndef INCLUDED_DISPLAY_DOT_H
#define INCLUDED_DISPLAY_DOT_H

#define STUB_DISPLAY 1  // Use fake display code

class Display
{
    float m_lastTemp;      //< Last temperature seen
    float m_lastHumidity;  //< Last humidity 

#if STUB_DISPLAY
    enum { bufferSize = 80 };
    char     m_buffer[bufferSize];  //< output buffer
#endif

  public:
    void initialize();
    void run();
};

extern Display theDisplay;

#endif // ! defined(INCLUDED_DISPLAY_DOT_H)
