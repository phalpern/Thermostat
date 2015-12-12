// Trace.t.cpp                  -*-C++-*-

#include "Trace.h"

int main()
{
    TRACE("First line");
    TRACE("This is a float: %5.2f", 3.5);
    int n = 15;
    TRACE("%%Starting loop of %ld repetitions", (long) n);
    TraceDump();
    for (int i = 0; i < n; ++i)
    {
        // TRACE("dual");
        TRACE("Iteration %d", i);
    }
    TRACE("Short string: \"%s\"", "Hello");
    TRACE("Long string: \"%s\"",
          "The quick brown fox jumped over the lazy dog");
    TRACE("100%% done");
    TraceDump();
}
