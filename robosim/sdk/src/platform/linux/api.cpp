#include <sys/stat.h>
#include "robosim/util/platform.h"


size_t GetAppPathA(char* filename, size_t size)
{
    return readlink ("/proc/self/exe",filename,size);
}


