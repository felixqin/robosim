#ifndef ODE_EXT_H__
#define ODE_EXT_H__

#include <ode/ode.h>
#include "../precomp.h"

//刚体平移,平移量为d(dx,dy,dz)
extern "C" SDK_API void BodyTranslate(dBodyID,float dx,float dy,float dz);

//刚体绕p点旋转,转动量为q(四元数)
//p = (px,py,pz)
//q = (qw,qx,qy,qz)
extern "C" SDK_API void BodyRotate(dBodyID,
                float px,float py,float pz,
                float qw,float qx,float qy,float qz);

//计算转动量在x-y平面投影的旋转角
extern "C" SDK_API float XYThetaForRotation(float qw,float qx, float qy, float qz);

#endif
