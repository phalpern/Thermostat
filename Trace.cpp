/* Trace.cpp                  -*-C++-*- */

#include "Trace.h"

#include <stdio.h>

TraceData::TraceData(const char*   data)
{
    strncpy(m_string, data, MaxString); 
}

struct TraceRecord
{
    unsigned short         m_repetitions;
    const TraceDescriptor* m_descriptor;
    TraceData              m_data;
};

namespace {

    const unsigned TraceMaxRecords = 32;  // Must be a power of 2

    TraceRecord theTraceRecords[TraceMaxRecords];

    unsigned theTraceCount = 0;    // Number of trace records
    unsigned theTraceLastDumpCount = 1; // Count as of last dump
    unsigned theTraceLastDumpRepetitions = 0; // Repeat count as of last dump

    // Parse the message for the first '%' formatting code and determine the
    // corresponding data type.
    TraceDataType dataTypeFromMessage(const char* message)
    {
        const char* formatChar_p = "";
        const char* percent = NULL;
        percent = strchr(message, '%');
        while (percent && percent[1] == '%')
            percent = strchr(percent + 1, '%');

        if (percent)
            formatChar_p = percent + strcspn(percent, "diuoxXfFeEgGaAcsp");

        switch (formatChar_p[0])
        {
          case '\0':
            return TraceTypeNone;
          case 'd':
          case 'i':
            if (formatChar_p[-1] == 'l')
                return TraceTypeLong;
            else
                return TraceTypeInt;
          case 'u':
          case 'o':
          case 'x':
          case 'X':
            if (formatChar_p[-1] == 'l')
                return TraceTypeUlong;
            else
                return TraceTypeUint;
          case 'f':
          case 'F':
          case 'e':
          case 'E':
          case 'g':
          case 'G':
          case 'a':
          case 'A':
            return TraceTypeDouble;
          case 'c':
            return TraceTypeInt;  // chars are passed as ints
          case 's':
            return TraceTypeString;
          case 'p':
            return TraceTypePointer;
          default:
            return TraceTypeUnknown;
        }
    }

    void TraceDumpRecord(const TraceRecord& record, unsigned startRepetitions)
    {
        int repetitions = record.m_repetitions - startRepetitions;
        if (0 == repetitions)
            return;

        printf("%s(%d): ", record.m_descriptor->m_file,
               record.m_descriptor->m_line);
        const char* message = record.m_descriptor->m_message;

        switch (record.m_descriptor->m_type)
        {
          case TraceTypeUnknown:
            printf("%s", message);
            break;
          case TraceTypeNone:
            printf(message);
            break;
          case TraceTypeInt:
            printf(message, record.m_data.m_int);
            break;
          case TraceTypeUint:
            printf(message, record.m_data.m_uint);
            break;
          case TraceTypeLong:
            printf(message, record.m_data.m_long);
            break;
          case TraceTypeUlong:
            printf(message, record.m_data.m_ulong);
            break;
          case TraceTypeDouble:
            printf(message, record.m_data.m_double);
            break;
          case TraceTypePointer:
            printf(message, record.m_data.m_pointer);
            break;
          case TraceTypeString: {
              char str[TraceData::MaxString + 1];
              strncpy(str, record.m_data.m_string, TraceData::MaxString);
              str[TraceData::MaxString] = '\0';
              printf(message, str);
            }
            break;
        }

        printf("\n");
        if (repetitions > 1)
            printf("     (above repeated %u times)\n", repetitions);
    }

} // End anonymous namespace

extern "C"
void TraceAddRecord(const TraceDescriptor* desc, TraceData data)
{
    unsigned index = 0;
    if (theTraceCount > 0) {
        index = theTraceCount % TraceMaxRecords;
        // If tracing the same line multiple times in a row, record
        // only the last one (even if data is different), but keep track
        // of repetition count.
        if (theTraceRecords[index].m_descriptor != desc) {
            // New descriptor, advance to a new slot
            index = ++theTraceCount % TraceMaxRecords;;
            theTraceRecords[index].m_repetitions = 0;
        }
    }
    else
        index = theTraceCount = 1;

    theTraceRecords[index].m_repetitions += 1;
    theTraceRecords[index].m_descriptor = desc;
    theTraceRecords[index].m_data = data;
}

extern "C"
void TraceAddRecordV(TraceDescriptor* desc, const char* msg, ...)
{
    if (desc->m_type == TraceTypeUnknown)
        desc->m_type = dataTypeFromMessage(msg);

    TraceData data;
    switch (desc->m_type)
    {
        // TBD: populate data based on type here
      default:
        break;
    }
    TraceAddRecord(desc, data);
}

extern "C"
void TraceDump()
{
    if (0 == theTraceCount)
        return; // No records at all

    // Dump records since added since last dump (but no more than
    // TraceMaxRecords records).
    unsigned startCount = theTraceLastDumpCount;
    unsigned startRepetitions = theTraceLastDumpRepetitions;
    if (theTraceCount - theTraceLastDumpCount > TraceMaxRecords)
    {
        startCount = theTraceCount - TraceMaxRecords + 1;
        startRepetitions = 0;
    }

    unsigned index;
    for (unsigned count = startCount; count <= theTraceCount; ++count)
    {
        index = count % TraceMaxRecords;
        TraceDumpRecord(theTraceRecords[index], startRepetitions);
        startRepetitions = 0;
    }

    theTraceLastDumpCount = theTraceCount;
    theTraceLastDumpRepetitions = theTraceRecords[index].m_repetitions;
}

/* End Trace.cpp */
