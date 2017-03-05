#ifndef PLATFORM_H__
#define PLATFORM_H__


#if defined(_WIN32)
#   define DLL_EXTNAME      ".dll"
#elif defined(LINUX) || defined(__linux__)
#   define DLL_EXTNAME      ".so"
#endif

#define DIRECTORY_SEPERATOR         '/'


#if defined(_WIN32)
#   include "win32/dll.h"
#   include "win32/api.h"
#elif defined(LINUX) || defined(__linux__)
#   include "linux/dll.h"
#   include "linux/api.h"
#endif

#endif
