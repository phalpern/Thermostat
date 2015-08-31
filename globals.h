/* globals.h                  -*-C++-*- */

#ifndef INCLUDED_GLOBALS_DOT_H
#define INCLUDED_GLOBALS_DOT_H

#include <application.h>  // Arduino functions and macros

/// @brief TempScale is used to select whether to read temp in C or F
enum TempScale { Celcius, Fahrenheit };

extern TempScale g_temp_scale;

#endif // ! defined(INCLUDED_GLOBALS_DOT_H)
