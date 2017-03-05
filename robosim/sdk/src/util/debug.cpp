#include <iostream>
#include <cstdio>
#include "robosim/util/debug.h"

#ifdef _WIN32
#   include "windows.h"
#else
    void OutputDebugStringA(const char* lpOutputString)
    {
        std::cerr<<lpOutputString<<std::endl;
    }
#endif

void DebugPrintA( const char* fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    static const int len = 260;
    char s[len];
    std::vsnprintf(s,len,fmt,ap);
    OutputDebugStringA(s);
    va_end(ap);
}
