#include <cstdio>
#include <cstdarg>
#include <string>
#include "robosim/util/log.h"

using namespace std;

string logfile = "log.txt";

void SetLogFileName(const char* fname)
{
    logfile = fname;
}

int LogData( const char* fmt, ...)
{
    int res = -1;
    FILE *fp = fopen(logfile.c_str(), "a");
    if (fp)
    {
        va_list ap;
        va_start(ap,fmt);
        res = vfprintf(fp,fmt,ap);
        fclose(fp);
        va_end(ap);
    }
    return res;
}
