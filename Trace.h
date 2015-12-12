/* Trace.h                  -*-C++-*- */

#ifndef INCLUDED_TRACE_DOT_H
#define INCLUDED_TRACE_DOT_H

#include <string.h>

#ifndef TRACE_ON
#   define TRACE_ON 1
#endif

enum TraceDataType {
    TraceTypeUnknown,
    TraceTypeNone,
    TraceTypeInt,
    TraceTypeUint,
    TraceTypeLong,
    TraceTypeUlong,
    TraceTypeDouble,
    TraceTypePointer,
    TraceTypeString
};

struct TraceDescriptor
{
    const char*   m_file;      ///< Source file of TRACE macro 
    unsigned      m_line : 24; ///< Line number of TRACE macro
    TraceDataType m_type : 8;  ///< Type of data displayed in trace
    const char*   m_message;   ///< Message string in printf format
};

/// Type whose size in bytes is a value of the `TraceDataType` enumeration.
template <TraceDataType Type> class TraceMetaType { char m_buffer[Type]; };

TraceMetaType<TraceTypeNone   > toTraceMetaType();
TraceMetaType<TraceTypeInt    > toTraceMetaType(signed   int );
TraceMetaType<TraceTypeUint   > toTraceMetaType(unsigned int );
TraceMetaType<TraceTypeUlong  > toTraceMetaType(signed   long);
TraceMetaType<TraceTypeLong   > toTraceMetaType(unsigned long);
TraceMetaType<TraceTypeDouble > toTraceMetaType(double       );
TraceMetaType<TraceTypePointer> toTraceMetaType(const void*  );
TraceMetaType<TraceTypeString > toTraceMetaType(const char*  );

/// @brief Record holding one 
union TraceData
{
    /// String data is truncated to this length.  Multiple of 8 recommended.
    enum { MaxString = 8 };

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
    TraceData(const char*   data);
};

#ifdef __cplusplus
extern "C" {
#endif
void TraceAddRecord(const TraceDescriptor* desc, TraceData data);

void TraceAddRecordV(TraceDescriptor* desc, const char* msg, ...)
    __attribute__((format(printf,2,3)));

void TraceDump();

#ifdef __cplusplus
} // close extern "C"
#endif

#define TRACE_DATA_TYPE(...)                                            \
    ((TraceDataType) sizeof(toTraceMetaType(__VA_ARGS__)))

#if ! TRACE_ON
# define TRACE(msg, ...) ((void) 0)
#elif defined(__cplusplus)
#  define TRACE(msg, ...)                                               \
    do {                                                                \
        static const TraceDescriptor trace_desc =                       \
            { __FILE__, __LINE__, TRACE_DATA_TYPE(__VA_ARGS__), msg };  \
        TraceAddRecord(&trace_desc, TraceData(__VA_ARGS__));            \
    } while (false)
#else // C
// TBD: C implementation is not yet complete
// Needs to distinguish between 1 and 2 args (dispatch to TRACE1/TRACE2.
#  define TRACE(msg, ...)                                               \
    do {                                                                \
        static TraceDescriptor trace_desc =                             \
            { __FILE__, __LINE__, TraceDataNone, msg };                 \
        TraceAddRecordC(&trace_desc, msg, __VA_ARGS__);                 \
    } while (false)
#endif // TRACE_ON

#endif // ! defined(INCLUDED_TRACE_DOT_H)
