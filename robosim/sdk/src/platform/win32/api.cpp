#include <windows.h>
#include "robosim/util/platform.h"


size_t GetAppPathA(char* filename, size_t size)
{
    return GetModuleFileName(0L,filename,size);
}


