#pragma once

#include "../precomp.h"

class TextureID;

class IApplication
{
public:
    virtual ~Application() {}

    virtual void init(int w,int h) = 0;
    virtual void loop() = 0;

    virtual TextureID* loadTexture( const char* filelist, int size);
}
