#pragma once

#include <vector>
#include <string>
#include <robosim/base/iresmgr.h>


class ResourceManager : public IResourceManager
{
public:
    static IResourceManager* instance();

public:
    ResourceManager();

    TextureID registerTexture(const char* filename);
    bool getTextureFilename(char filename[], TextureID);

    TextureID getMaxTextureID() const { return (int)tex_table_.size()-1; }

protected:
    int find_filename(const char* filename);

private:
    std::string tex_path_;
    std::vector<std::string> tex_table_;
};

inline
IResourceManager* ResourceManager::instance()
{
    static ResourceManager rm;
    return &rm;
}
