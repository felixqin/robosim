#include "glfw/gl.h"
#include "glfw/callback.h"

namespace glfw
{

WindowSizeFunction      funcWindowSize;
WindowCloseFunction     funcWindowClose;
WindowRefreshFunction   funcWindowRefresh;

KeyFunction             funcKey;
CharFunction            funcChar;
MouseButtonFunction     funcMouseButton;
MousePosFunction        funcMousePos;
MouseWheelFunction      funcMouseWheel;


void GLFWCALL cbfuncWindowSize(int width, int height)
{
    funcWindowSize(width,height);
}
int GLFWCALL cbfuncWindowClose()
{
    return funcWindowClose();
}
void GLFWCALL cbfuncWindowRefresh()
{
    funcWindowRefresh();
}
void GLFWCALL cbfuncKey(int key, int action)
{
    funcKey(key,action);
}
void GLFWCALL cbfuncChar(int character, int action)
{
    funcChar(character,action);
}
void GLFWCALL cbfuncMouseButton(int button, int action)
{
    funcMouseButton(button,action);
}
void GLFWCALL cbfuncMousePos(int x, int y)
{
    funcMousePos(x,y);
}
void GLFWCALL cbfuncMouseWheel(int pos)
{
    funcMouseWheel(pos);
}


void setWindowSizeCallback(WindowSizeFunction func)
{
    if (!func)
    {
        glfwSetWindowSizeCallback(NULL);
    }
    else
    {
        funcWindowSize = func;
        glfwSetWindowSizeCallback(cbfuncWindowSize);
    }
}
void setWindowCloseCallback(WindowCloseFunction func)
{
    if (!func)
    {
        glfwSetWindowCloseCallback(NULL);
    }
    else
    {
        funcWindowClose = func;
        glfwSetWindowCloseCallback(cbfuncWindowClose);
    }
}
void setWindowRefreshCallback(WindowRefreshFunction func)
{
    if (!func)
    {
        glfwSetWindowRefreshCallback(NULL);
    }
    else
    {
        funcWindowRefresh = func;
        glfwSetWindowRefreshCallback(cbfuncWindowRefresh);
    }
}
void setKeyCallback(KeyFunction func)
{
    if (!func)
    {
        glfwSetKeyCallback(NULL);
    }
    else
    {
        funcKey = func;
        glfwSetKeyCallback(cbfuncKey);
    }
}
void setCharCallback(CharFunction func)
{
    if (!func)
    {
        glfwSetCharCallback(NULL);
    }
    else
    {
        funcChar = func;
        glfwSetCharCallback(cbfuncChar);
    }
}
void setMouseButtonCallback(MouseButtonFunction func)
{
    if (!func)
    {
        glfwSetMouseButtonCallback(NULL);
    }
    else
    {
        funcMouseButton = func;
        glfwSetMouseButtonCallback(cbfuncMouseButton);
    }
}
void setMousePosCallback(MousePosFunction func)
{
    if (!func)
    {
        glfwSetMousePosCallback(NULL);
    }
    else
    {
        funcMouseButton = func;
        glfwSetMousePosCallback(cbfuncMousePos);
    }
}
void setMouseWheelCallback(MouseWheelFunction func)
{
    if (!func)
    {
        glfwSetMouseWheelCallback(NULL);
    }
    else
    {
        funcMouseWheel = func;
        glfwSetMouseWheelCallback(cbfuncMouseWheel);
    }
}


}//namespaces
