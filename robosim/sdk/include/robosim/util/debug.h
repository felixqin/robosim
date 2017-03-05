#pragma once

#include "../precomp.h"

extern "C" SDK_API void DebugPrintA(const char* fmt, ...);


#ifdef TRACE
#undef TRACE
#endif

#ifdef _DEBUG
#   ifdef _UNICODE
#       error not implement
#       define  TRACE   DebugPrintW
#   else
#       define  TRACE   DebugPrintA
#   endif
#else
#   define  TRACE
#endif
