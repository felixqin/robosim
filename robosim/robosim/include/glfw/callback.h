#ifndef GLFW_CALLBACK_H__
#define GLFW_CALLBACK_H__

#include <functional>


namespace glfw
{

typedef std::tr1::function<void (int, int) > WindowSizeFunction;
typedef std::tr1::function<int (void) > WindowCloseFunction;
typedef std::tr1::function<void (void) > WindowRefreshFunction;

typedef std::tr1::function<void (int, int) > KeyFunction;
typedef std::tr1::function<void (int, int) > CharFunction;
typedef std::tr1::function<void (int, int) > MouseButtonFunction;
typedef std::tr1::function<void (int, int) > MousePosFunction;
typedef std::tr1::function<void (int) > MouseWheelFunction;



void setWindowSizeCallback(WindowSizeFunction func);
void setWindowCloseCallback(WindowCloseFunction func);
void setWindowRefreshCallback(WindowRefreshFunction func);

void setKeyCallback(KeyFunction func);
void setCharCallback(CharFunction func);
void setMouseButtonCallback(MouseButtonFunction func);
void setMousePosCallback(MousePosFunction func);
void setMouseWheelCallback(MouseWheelFunction func);

}

#endif //GLFW_CALLBACK_H__
