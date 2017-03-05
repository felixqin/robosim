#pragma once

namespace rgcp
{

template<unsigned c1, unsigned c2=0, unsigned c3=0, unsigned c4=0>
class to_tag
{
public:
    enum { value = ((c4*256+c3)*256+c2)*256+c1 };
};


/******************************************************************************/

typedef unsigned long int tag;

template<typename StringT>
inline
tag tag_from_string( StringT s)
{
    unsigned long int n = 0;
    for ( int i=0; i<4 && s[i]!=0; ++i )
    {
        unsigned long int c = s[i];
        n += c<<(i*8);
    }
    return n;
}

inline
std::string tag_to_string(tag t)
{
    tag ta[]={t,0};
    char* s = (char*)&ta;
    return s;
}

}
