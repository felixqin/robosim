#include <cmath>
#include "robosim/util/odeext.h"
#include "robosim/util/debug.h"

//刚体平移,平移量为d(dx,dy,dz)
void BodyTranslate(dBodyID body,float dx,float dy,float dz)
{
    const dReal* p = dBodyGetPosition(body);
    dBodySetPosition(body,p[0]+dx,p[1]+dy,p[2]+dz);
}

//刚体绕定点p旋转,转动量为q(四元数)
//p = (px,py,pz)
//q = (qw,qx,qy,qz)
//
//new_pos = q(old_pos-p)q^-1+p
//   => new_dp = q*old_dp*q^-1  (new_dp=new_pos-p, old_dp=old_pos-p)
//new_qua = q*old_qua
//详细证明见文档及附件5
void BodyRotate(dBodyID body,
                float px,float py,float pz,
                float qw,float qx,float qy,float qz)
{
    const dReal* op = dBodyGetPosition(body);
    const dReal* oq = dBodyGetQuaternion(body);

    dQuaternion q = { qw, qx, qy, qz };
    dQuaternion odp = { 0, op[0]-px, op[1]-py, op[2]-pz };

    dQuaternion ndp,nq,tq;

    dQMultiply0(tq,q,odp);  // tq = q*old_dq
    dQMultiply2(ndp,tq,q);  // new_dp = tq*q^-1
    dQMultiply0(nq,q,oq);   // new_qua = q*old_qua

    dBodySetPosition(body,ndp[1]+px,ndp[2]+py,ndp[3]+pz);
    dBodySetQuaternion(body,nq);
}

//计算转动量在x-y平面投影的旋转角
//计算过程描述见文档附件9
float XYThetaForRotation(float qw,float qx, float qy, float qz)
{
    if (qw-1.0f<0.0001f&&qw-1.0f>-0.0001f)  // 没有旋转
    {
        return 0.0f;
    }

    TRACE("q: %f,%f,%f,%f",qw,qx,qy,qz);
    //u=(0,u1,u2,u3)=q*i3*(q^-1)        ; i3=(0,0,0,1) 即z轴方向的单位向量
    dReal u1 = (qx*qz+qw*qy)*2;
    dReal u2 = (-qw*qx+qy*qz)*2;
    //dReal u3 = qw*qw-qx*qx-qy*qy+qz*qz;
    TRACE("u: 0,%f,%f,unknown",u1,u2);

    //v = u (cross) i3 = (0,u2,-u1,0)
    dReal v1 = u2;
    dReal v2 = -u1;

    //m是v的单位向量
    dReal lv = std::sqrt(v1*v1+v2*v2);
    dReal m1 = v1/lv;
    dReal m2 = v2/lv;
    TRACE("m: 0,%f,%f,0",m1,m2);

    dReal cos_half_phi = std::sqrt(qw*qw+qz*qz);
    dReal sin_half_phi = std::sqrt(qx*qx+qy*qy);
    TRACE("cos/sin(phi/2): %f,%f",cos_half_phi,sin_half_phi);

    //dQuaternion p = { cos_half_phi, m1*sin_half_phi, m2* sin_half_phi, 0 };
    //dQuaternion q = { qw, qx, qy, qz };
    //dQuaternion r;
    //TRACE("p: %f,%f,%f,0",p[0],p[1],p[2]);
    dReal pw = cos_half_phi;
    dReal px = m1*sin_half_phi;
    dReal py = m2*sin_half_phi;
    //dReal pz = 0.0f;

    //dQMultiply0(r,p,q); //r=p*q;    r=[cos(theta/2),0,0,sin(theta/2)]
    //TRACE("r: %f,%f,%f,%f",r[0],r[1],r[2],r[3]);
    //rw = p0*q0-p1*q1-p2*q2-p3*q3;
    //rz = p0*q3+p3*q0+p1*q2-p2*q1;
    //use manual r0 and r3
    dReal rw = pw*qw-px*qx-py*qy/*-pz*qz*/;
    dReal rz = pw*qz/*+pz*qw*/+px*qy-py*qx;

    dReal a=atan2(rz,rw)*2;
    TRACE("a: %f",a);
    return a;
}
