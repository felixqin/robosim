#ifndef CAMERA_H__
#define CAMERA_H__

class Camera
{
public:
    Camera();
    Camera(float eyex, float eyey, float eyez,
        float centerx, float centery, float centerz,
        float upx, float upy, float upz);

    const float* getPosition() const;
    const float* getUp() const;
    const float* getFront() const;

    void moveFront(float d);
    void moveRight(float d);
    void moveUp(float d);
    void turnUp(float da);
    void turnLeft(float da);

    void horizontalize();

    void roundLeft(float da);
    void roundTop(float da);

protected:
    void move(float n[], float d);

private:
    float p_[3];
    float u_[3];
    float r_[3];
    float f_[3];

protected:
    static void normalize(float v[]);
    static void cross(const float left[], const float right[], float result[]);
    /**
    * v 沿 v和v_end构成的平面, 向v_end旋转angle弧度
    * v 正交于 v_end, v v_end 是单位向量
    */
    static void rotate(float v[], const float v_end[], const float angle);
    /**
    * Schmidt正交化两个向量(a1,a2), 结果为 (a1,b2).
    * 要求a1必须为单位向量
    */
    static void schmidt2(const float a1[], const float a2[], float b2[]);
};

inline
const float* Camera::getPosition() const
{
    return p_;
}
inline
const float* Camera::getUp() const
{
    return u_;
}
inline
const float* Camera::getFront() const
{
    return f_;
}
/******************************************************/
inline
void Camera::moveFront(float d)
{
    move(f_,d);
}

/******************************************************/
inline
void Camera::moveRight(float d)
{
    move(r_,d);
}

/******************************************************/
inline
void Camera::moveUp(float d)
{
    move(u_,d);
}

/******************************************************/
inline
void Camera::turnUp(float da)
{
    rotate(f_,u_,da);
    cross(r_,f_,u_);
}

/******************************************************/
inline
void Camera::turnLeft(float da)
{
    rotate(r_,f_,da);
    cross(u_,r_,f_);
}
/******************************************************/
inline
void Camera::horizontalize()
{
    static const float ez[] = { 0, 0, 1 };
    schmidt2(f_,ez,u_);
    normalize(u_);
    cross(f_,u_,r_);
}


#endif
