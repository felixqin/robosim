#pragma once

#include "../precomp.h"


typedef int TextureID;

static const TextureID TEXTURE_NONE = -1;
static const TextureID TEXTURE_GROUND = 0;
static const TextureID TEXTURE_SKY = 1;
static const TextureID TEXTURE_WOOD = 2;
static const TextureID TEXTURE_STONE = 3;


class IResourceManager
{
public:
    virtual ~IResourceManager() {}

    virtual TextureID registerTexture(const char* filename) = 0;
    virtual bool getTextureFilename(char filename[], TextureID) = 0;

    virtual TextureID getMaxTextureID() const = 0;
};

