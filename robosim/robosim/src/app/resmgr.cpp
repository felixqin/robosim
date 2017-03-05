#include <cstring>
#include "app/resmgr.h"
#include "app/configure.h"

ResourceManager::ResourceManager()
{
    registerTexture("ground.ppm");
    registerTexture("sky.ppm");
    registerTexture("wood.ppm");
    registerTexture("stone.ppm");

    tex_path_ = Configure::instance()->getSysParam().res_path;
    tex_path_ += "/";
}

int ResourceManager::find_filename(const char* filename)
{
    for ( int i = 0; i<tex_table_.size(); ++i )
    {
        if (tex_table_[i]==filename)
        {
            return i;
        }
    }
    return -1;

}

TextureID ResourceManager::registerTexture(const char* filename)
{

    TextureID id = static_cast<TextureID>(find_filename(filename));
    if ( id < 0 )
    {
        tex_table_.push_back(filename);
        id = static_cast<TextureID>(tex_table_.size()-1);
    }
    return id;
}

bool ResourceManager::getTextureFilename(char filename[], TextureID id)
{
    try
    {
        std::strcpy(filename,(tex_path_+tex_table_[id]).c_str());
    }
    catch(const std::exception&)
    {
        return false;
    }
    return true;
}
