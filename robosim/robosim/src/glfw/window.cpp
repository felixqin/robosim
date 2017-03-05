#include <functional>
#include "glfw/gl.h"
#include "glfw/callback.h"
#include "glfw/window.h"

using namespace glfw;

Window::Window()
    :
    drawing_(0),
    //camera_( 5,0.5,0.5,4,0.5,0.5,0,0,1),
    df(0),dr(0),dau(0),dal(0),
    turbo(1)
{
    glfwInit();
}
Window::~Window()
{
    glfwTerminate();
}

bool Window::open(int w,int h)
{
    if (glfwOpenWindow(w,h,0,0,0,0,16,0,GLFW_WINDOW)==GL_FALSE)
    {
        return false;
    }
    glfwSetWindowTitle("robosim");

    drawing_ = new Drawing();
    //drawing_->enableTexture(false);

    camera_.moveFront(-6);

    using namespace std::tr1;
    using namespace std::tr1::placeholders;
    setWindowSizeCallback(bind(&Window::onsize,this,_1,_2));
    setKeyCallback(bind(&Window::onkey,this,_1,_2));

    return true;
}

IDrawing* Window::beginPaint()
{
    control();

    //TODO delete test code
//    int width,height;
//    glfwGetWindowSize( &width, &height );
//    height = height > 0 ? height : 1;
//    drawing_->setSize(width,height);

    drawing_->beginFrame();
    //test();
    return drawing_;
}
void Window::endPaint()
{
    drawing_->endFrame();
    glfwSwapBuffers();
}

bool Window::closed()
{
    return !glfwGetWindowParam(GLFW_OPENED);
}

void Window::onsize(int w,int h)
{
    drawing_->setSize(w,h);
}

void Window::onkey(int key,int action)
{
    float da = 0, de = 0;

    bool press = (action == GLFW_PRESS );

    if (press)
    {
        da = 0.02;
        de = 0.02;
    }

    switch(key)
    {
    case GLFW_KEY_LCTRL:
        turbo = press ? 5 : 1;
        break;

    case GLFW_KEY_KP_8:
        dau= da;
        break;
    case GLFW_KEY_KP_2:
        dau= -da;
        break;
    case GLFW_KEY_KP_4:
        dal= da;
        break;
    case GLFW_KEY_KP_6:
        dal= -da;
        break;

    case GLFW_KEY_LEFT:
        dr= -de;
        break;
    case GLFW_KEY_RIGHT:
        dr= de;
        break;
    case GLFW_KEY_UP:
        df= de;
        break;
    case GLFW_KEY_DOWN:
        df= -de;
        break;

    case GLFW_KEY_HOME:
        camera_.horizontalize();
        break;
    }
}

void Window::control()
{
    camera_.roundLeft(dal*turbo);
    camera_.roundTop(dau*turbo);
    camera_.moveFront(df*turbo);
    camera_.moveRight(dr*turbo);

    const float* pos = camera_.getPosition();
    const float* front = camera_.getFront();
    const float* up = camera_.getUp();
    drawing_->setCamera(pos,front,up);
}

