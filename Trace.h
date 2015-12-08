/* Trace.h                  -*-C++-*- */

#ifndef INCLUDED_TRACE_DOT_H
#define INCLUDED_TRACE_DOT_H

#include <string.h>

struct TraceDescriptor
{
    const char* m_file;     ///< Source file of TRACE macro 
    unsigned    m_line;     ///< Line number of TRACE macro
    const char* m_message;  ///< Message string in printf format
};

/// @brief Record holding one 
union TraceData
{
    enum { MaxString = 16 };

    signed   int  m_int; 
    unsigned int  m_uint;
    signed   long m_long;
    unsigned long m_ulong;
    double        m_double;       
    const void*   m_pointer;  
    char          m_string[MaxString];

    TraceData()                   : m_int(0) { } 
    TraceData(signed   int  data) : m_int(data) { } 
    TraceData(unsigned int  data) : m_uint(data) { }
    TraceData(signed   long data) : m_ulong(data) { }
    TraceData(unsigned long data) : m_long(data) { }
    TraceData(double        data) : m_double(data) { }       
    TraceData(const void*   data) : m_pointer(data) { }  
    TraceData(const char*   data) { strncpy(m_string, data, MaxString); }
};

void TraceAddRecord(const TraceDescriptor* desc, TraceData data);

void TraceDump();

#define TRACE(msg, ...)                                                  \
    do {                                                                 \
        static TraceDescriptor trace_desc = { __FILE__, __LINE__, msg }; \
        TraceAddRecord(&trace_desc, TraceData(__VA_ARGS__));             \
    } while (false)

#endif // ! defined(INCLUDED_TRACE_DOT_H)
