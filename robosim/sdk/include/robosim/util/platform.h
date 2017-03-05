#pragma once

#include <string>

#include "../precomp.h"



extern "C" SDK_API size_t GetAppPathA(char* filename, size_t size);


#ifndef MAX_PATH
#define MAX_PATH    260
#endif

inline
size_t GetAppPath(std::string& path)
{
    path.resize(MAX_PATH);
    return GetAppPathA(&path[0],MAX_PATH);
}
