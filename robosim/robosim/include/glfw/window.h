#ifndef WINDOW_H__
#define WINDOW_H__

#include "app/isimui.h"
#include "util/camera.h"
#include "gl.h"
#include "drawing.h"

namespace glfw
{

class Window : public ISimUI
{
public:
    Window();
    ~Window();

    bool open(int w=640,int h=480);
    bool closed();
    void sleep(double sec);
    IDrawing* beginPaint();
    void endPaint();

    IDrawing* getDrawing() { return drawing_; }

    void onkey(int key,int action);
	void onsize(int w,int h);

protected:
    void control();

private:
    Drawing* drawing_;
    Camera camera_;

    float df;
    float dr;
    float dau;
    float dal;
    float turbo;

};


inline void Window::sleep(double sec)
{
    glfwSleep(sec);
}

}//namespace

#endif
