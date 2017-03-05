#pragma once

//#include <tr1/cinttypes>

#include "precomp.h"

//typedef std::tr1::uint32_t Tag;

typedef unsigned long int Tag;

inline Tag MakeTag( const char* tag)
{
    unsigned long int n = 0;
    for ( int i=0; i<4 && tag[i]!=0; ++i )
    {
        unsigned long int t = tag[i];
        n += t<<(i*8);
    }
    return n;
}


typedef float Real;
