#ifndef IDRAWING_H__
#define IDRAWING_H__

#include "../precomp.h"
#include "iresmgr.h"

class IDrawing
{
public:
    virtual ~IDrawing() {}

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual void pushMatrix() = 0;
    virtual void popMatrix() = 0;
    //坐标变换,平移量(px,py,pz),旋转量(qw,qx,qy,qz)(四元数)
    virtual void transform(float px,float py,float pz,float qw,float qx,float qy,float qz) = 0;

    virtual void setCamera(const float* pos, const float* front, const float* up) = 0;
    virtual void setColor(float r,float g,float b,float alpha) = 0;
    virtual void setTexture(TextureID) = 0;

    virtual void drawRectangle(float a, float b) = 0;
    virtual void drawBox(float lx, float ly, float lz) = 0;
    virtual void drawSphere(float radius) = 0;
    virtual void drawCylinder(float radius, float length) = 0;
    virtual void drawTrimesh(const float* vertices, const unsigned int* indices, size_t index_count) = 0;

    virtual void setSphereQuality(int n) = 0;
    virtual void setCapsuleQuality(int n) = 0;
};


#endif //DRAWING_H__
