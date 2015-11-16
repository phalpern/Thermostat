/* Settings.h                  -*-C++-*- */

#ifndef INCLUDED_SETTINGS_DOT_H
#define INCLUDED_SETTINGS_DOT_H

#include <application.h>  // Arduino functions and macros

/// @brief TempScale is used to select whether to read temp in C or F
enum TempScale { Celcius, Fahrenheit };

extern TempScale g_tempScale;
extern float g_currentTempCF; //current temperature represented in the current temp scale (C or F)


#endif // ! defined(INCLUDED_SETTINGS_DOT_H)
