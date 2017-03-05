#include <stdexcept>
#include <robosim/util/debug.h>
#include "glfw/gl.h"
#include "glfw/drawing.h"
#include "glfw/texture.h"
#include "glfw/drawutil.h"
#include "app/resmgr.h"

IResourceManager* res_mgr = ResourceManager::instance();

static const float LIGHTX = 0, LIGHTY = 0, LIGHTZ = 2;

namespace glfw
{

Drawing::Drawing()
    :
    width_(640),
    height_(480),
    cur_tex_(0),
    texture_enabled_(true)
{
    setSphereQuality(1);

    //加载所有纹理
    TextureID max_id = res_mgr->getMaxTextureID();
    for ( TextureID id = 0; id <= max_id; ++id )
    {
        load_texture(id);
    }
}

Drawing::~Drawing()
{
    textures_.clear();  //先释放纹理
}

Texture* Drawing::load_texture(TextureID id)
{
    TRACE("load texture:");

    Texture* tex = 0;

    char filename[256];
    if (res_mgr->getTextureFilename(filename,id))
    {
        TRACE("%s\n",filename);
        tex = new Texture(filename);
        if ( textures_.size() <= id )
        {
            textures_.resize(id+1);
        }
        typedef std::tr1::shared_ptr<Texture> TextureSP;
        TextureSP tex_sp = TextureSP(tex);
        textures_[id].swap(tex_sp);
    }
    return tex;
}


void Drawing::draw_ground()
{
    if ( texture_enabled_ )
    {
        setTexture(TEXTURE_GROUND);
        if (cur_tex_) cur_tex_->bind(0);
    }

    static const float ground_scale = 1.0f/2.0f;	// ground texture scale (1/size)
    static const float ground_ofsx = 0.5;		// offset of ground texture
    static const float ground_ofsy = 0.5;

    static const float gsize = 100.0f;
    static const float offset = -0.001f; // -0.001f; ... polygon offsetting doesn't work well

    glBegin (GL_QUADS);
    glNormal3f (0,0,1);
    glTexCoord2f (-gsize*ground_scale + ground_ofsx,
        -gsize*ground_scale + ground_ofsy);
    glVertex3f (-gsize,-gsize,offset);
    glTexCoord2f (gsize*ground_scale + ground_ofsx,
        -gsize*ground_scale + ground_ofsy);
    glVertex3f (gsize,-gsize,offset);
    glTexCoord2f (gsize*ground_scale + ground_ofsx,
        gsize*ground_scale + ground_ofsy);
    glVertex3f (gsize,gsize,offset);
    glTexCoord2f (-gsize*ground_scale + ground_ofsx,
        gsize*ground_scale + ground_ofsy);
    glVertex3f (-gsize,gsize,offset);
    glEnd();

}
void Drawing::draw_sky()
{
    if ( texture_enabled_ )
    {
        setTexture(TEXTURE_SKY);
        if (cur_tex_) cur_tex_->bind(0);
    }

    static const float sky_scale = 1.0f/4.0f;	// sky texture scale (1/size)
    static const float sky_height = 1.0f;		// sky height above viewpoint

    // make sure sky depth is as far back as possible
    glShadeModel (GL_FLAT);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    glDepthRange (1,1);

    static const float ssize = 1000.0f;
    static float offset = 0.0f;

    float view_xyz[] = { eye_[0], eye_[1], eye_[2] };
    float x = ssize*sky_scale;
    float z = view_xyz[2] + sky_height;

    glBegin (GL_QUADS);
    glNormal3f (0,0,-1);
    glTexCoord2f (-x+offset,-x+offset);
    glVertex3f (-ssize+view_xyz[0],-ssize+view_xyz[1],z);
    glTexCoord2f (-x+offset,x+offset);
    glVertex3f (-ssize+view_xyz[0],ssize+view_xyz[1],z);
    glTexCoord2f (x+offset,x+offset);
    glVertex3f (ssize+view_xyz[0],ssize+view_xyz[1],z);
    glTexCoord2f (x+offset,-x+offset);
    glVertex3f (ssize+view_xyz[0],-ssize+view_xyz[1],z);
    glEnd();

    offset = offset + 0.002f;
    if (offset > 1) offset -= 1;

    glDepthFunc (GL_LESS);
    glDepthRange (0,1);
}

}
