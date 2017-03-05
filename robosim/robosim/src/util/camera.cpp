#include "util/camera.h"
#include <cmath>


/******************************************************/
Camera::Camera()
{
    p_[0] = 0.0f;
    p_[1] = 0.0f;
    p_[2] = 0.0f;

    r_[0] = 1.0f;
    r_[1] = 0.0f;
    r_[2] = 0.0f;

    f_[0] = 0.0f;
    f_[1] = 0.0f;
    f_[2] = -1.0f;

    u_[0] = 0.0f;
    u_[1] = 1.0f;
    u_[2] = 0.0f;
}
/******************************************************/
Camera::Camera(float eyex, float eyey, float eyez,
        float centerx, float centery, float centerz,
        float upx, float upy, float upz)
{
    p_[0] = eyex;
    p_[1] = eyey;
    p_[2] = eyez;

    f_[0] = centerx - eyex;
    f_[1] = centery - eyey;
    f_[2] = centerz - eyez;

    u_[0] = upx;
    u_[1] = upy;
    u_[2] = upz;

    normalize(f_);
    normalize(u_);
    cross(f_,u_,r_);
}

/******************************************************/
void Camera::move(float n[], float d)
{
    p_[0] += d*n[0];
    p_[1] += d*n[1];
    p_[2] += d*n[2];
}
/******************************************************/
void Camera::normalize(float v[])
{
    float s = std::sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    v[0] /= s;
    v[1] /= s;
    v[2] /= s;
}
void Camera::cross(const float left[], const float right[], float result[])
{
    result[0] = left[1]*right[2] - left[2]*right[1];
    result[1] = left[2]*right[0] - left[0]*right[2];
    result[2] = left[0]*right[1] - left[1]*right[0];
}
/**
* v 沿 v和v_end构成的平面, 向v_end旋转angle弧度
* v 正交于 v_end, v v_end 是单位向量
*/
void Camera::rotate(float v[], const float v_end[], const float angle)
{
    float cosa = std::cos(angle);
    float sina = std::sin(angle);
    v[0] = v[0]*cosa + v_end[0]*sina;
    v[1] = v[1]*cosa + v_end[1]*sina;
    v[2] = v[2]*cosa + v_end[2]*sina;
}
/**
* Schmidt正交化两个向量a1,a2, 结果为 a1,b2;
* 要求a1必须为单位向量
*/
void Camera::schmidt2(const float a1[], const float a2[], float b2[])
{
    float k1 = ( a1[0]*a2[0]+a1[1]*a2[1]+a1[2]*a2[2] );
    b2[0] = a2[0] - k1*a1[0];
    b2[1] = a2[1] - k1*a1[1];
    b2[2] = a2[2] - k1*a1[2];
}

/***************************************************************************/
void Camera::roundTop(float da)
{
    if (da==0 ) return;

    float k = (f_[2]==0) ? 0 : - p_[2]/f_[2];

    float fo[3];
    fo[0] = f_[0];
    fo[1] = f_[1];
    fo[2] = f_[2];

    turnUp(-da);

    p_[0] += k*(fo[0]-f_[0]);
    p_[1] += k*(fo[1]-f_[1]);
    p_[2] += k*(fo[2]-f_[2]);
}

void Camera::roundLeft(float da)
{
    if (da==0) return;

    float k = (f_[2]==0) ? 0 : - p_[2]/f_[2];

    float cosa = std::cos(da);
    float sina = std::sin(da);
    float df[2];
    df[0] =  f_[0]*cosa + f_[1]*sina - f_[0];
    df[1] = -f_[0]*sina + f_[1]*cosa - f_[1];

    p_[0] -= k*df[0];
    p_[1] -= k*df[1];

    f_[0] += df[0];
    f_[1] += df[1];

    horizontalize();
}
