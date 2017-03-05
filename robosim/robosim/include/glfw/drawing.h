#ifndef GL_DRAWING_H__
#define GL_DRAWING_H__

#include <vector>
#include <memory>
#include <robosim/base/idrawing.h>
#include "gl.h"
#include "drawutil.h"
#include "texture.h"

namespace glfw
{

class Texture;

class Drawing : public IDrawing
{
public:
    Drawing();
    virtual ~Drawing();

    void enableTexture(bool enable=true) { texture_enabled_ = enable; }

    void beginFrame();
    void endFrame();

    void setSize(int w,int h);
    void setCamera(const float* pos, const float* front, const float* up);
    void setColor(float r,float g,float b,float alpha);
    void setTexture(TextureID id);
    void setSphereQuality(int n);
    void setCapsuleQuality (int n);

    void pushMatrix();
    void popMatrix();
    void transform(float px,float py,float pz,float qw,float qx,float qy,float qz);

    void drawRectangle(float a, float b);
    void drawBox(float lx, float ly, float lz);
    void drawSphere(float radius);
    void drawCylinder(float radius, float length);
    void drawTrimesh(const float* vertices, const unsigned int* indices, size_t index_count);

protected:
    Texture* load_texture(TextureID);
    void predraw();

    void draw_ground();
    void draw_sky();

private:
    DrawUtility draw_;

    int width_,height_;
    float eye_[3];
    float center_[3];
    float up_[3];

    GLint sphere_slices_;
    GLint sphere_stacks_;

    std::vector<std::tr1::shared_ptr<Texture> > textures_;
    Texture*    cur_tex_;  //当前纹理
    GLfloat     cur_color_[4];

    bool texture_enabled_;
};



////////////////////////////////////////////////////////////////////////////////
// 使用内联函数加快绘图速度
////////////////////////////////////////////////////////////////////////////////

inline
void OGLfromPQ(float* M,float px,float py,float pz,float qw,float qx,float qy,float qz)
{
    float qxx = 2*qx*qx;
    float qyy = 2*qy*qy;
    float qzz = 2*qz*qz;
    float qxy = 2*qx*qy;
    float qyz = 2*qy*qz;
    float qzx = 2*qz*qx;
    float qwx = 2*qw*qx;
    float qwy = 2*qw*qy;
    float qwz = 2*qw*qz;
    M[ 0] = 1-qyy-qzz;  M[ 4] = qxy-qwz;    M[ 8] = qzx+qwy;    M[12] = px;
    M[ 1] = qxy+qwz;    M[ 5] = 1-qxx-qzz;  M[ 9] = qyz-qwx;    M[13] = py;
    M[ 2] = qzx-qwy;    M[ 6] = qyz+qwx;    M[10] = 1-qxx-qyy;  M[14] = pz;
    M[ 3] = 0.0f;       M[ 7] = 0.0f;       M[11] = 0.0f;       M[15] = 1.0f;
}

inline
void Drawing::transform(float px,float py,float pz,float qw,float qx,float qy,float qz)
{
    float M[16];
    OGLfromPQ(M,px,py,pz,qw,qx,qy,qz);
    glMultMatrixf(M);
}

inline
void Drawing::pushMatrix()
{
    glPushMatrix();
}

inline
void Drawing::popMatrix()
{
    glPopMatrix();
}

/******************************************************************************/

inline
void Drawing::setSize(int width, int height)
{
    width_ = width;
    height_ = height;
}

inline
void Drawing::setTexture(TextureID id)
{
    cur_tex_ = 0;
    if (  id>=0 && id<textures_.size() )
    {
        cur_tex_ = textures_[id].get();
        if (!cur_tex_)
        {
            //按需加载纹理
            cur_tex_ = load_texture(id);
        }
    }
}

inline
void Drawing::setCamera(const float* pos, const float* front, const float* up)
{
    std::copy(&pos[0],&pos[3],&eye_[0]);
    std::copy(&up[0],&up[3],&up_[0]);
    center_[0] = pos[0]+front[0];
    center_[1] = pos[1]+front[1];
    center_[2] = pos[2]+front[2];
}

inline
void Drawing::setColor(float r,float g,float b,float alpha)
{
  cur_color_[0] = r;
  cur_color_[1] = g;
  cur_color_[2] = b;
  cur_color_[3] = 1;
}

inline
void Drawing::setSphereQuality(int n)
{
    sphere_slices_ = sphere_stacks_ = 15 + n*5;
}
inline
void Drawing::setCapsuleQuality (int n)
{
    sphere_slices_ = sphere_stacks_ = 15 + n*5;
}


/******************************************************************************/

inline
void Drawing::beginFrame()
{
    draw_.resize(width_,height_);
    draw_.frameStart();

    gluLookAt(eye_[0],eye_[1],eye_[2],
            center_[0],center_[1],center_[2],
            up_[0],up_[1],up_[2]);

    //draw_ground();
    //draw_sky();
    setTexture(TEXTURE_WOOD);
}

inline
void Drawing::endFrame()
{
    draw_.frameEnd();
}

inline
void Drawing::predraw()
{
    if ( texture_enabled_ && cur_tex_)
    {
        glEnable(GL_TEXTURE_2D);
    	cur_tex_->bind(1);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }
    glColor4fv(cur_color_);
}

inline
void Drawing::drawRectangle(float a, float b)
{
    predraw();
    draw_.drawRectangle(a,b);
}
inline
void Drawing::drawBox (float lx, float ly, float lz)
{
    predraw();
    draw_.drawBox(lx,ly,lz);
}

inline
void Drawing::drawSphere(float radius)
{
    predraw();
    draw_.drawSphere(radius,sphere_slices_,sphere_stacks_);
}

inline
void Drawing::drawCylinder(float radius, float length)
{
    predraw();
    draw_.drawCylinder(radius,length,sphere_slices_,sphere_stacks_);
}

inline
void Drawing::drawTrimesh(const float* vertices,
        const unsigned int* indices, size_t index_count)
{
    predraw();
    draw_.drawTrimesh(vertices,indices,index_count);
}


}//namespace

#endif //GL_DRAWING_H__

