#include <robosim/base/itimer.h>
#include "gl.h"

namespace glfw
{
    class Timer : public ITimer
    {
        double start_,cumulation_;
        bool paused_;
    public:
        Timer() : paused_(true), cumulation_(0.0f)
        {
        }
        double elapsed() const
        {
            return (paused_)? cumulation_ : glfwGetTime()-start_ + cumulation_;
        }
        void reset()
        {
            paused_ = true;
            cumulation_ = 0.0f;
        }
        void start()
        {
            if (paused_)
            {
                start_ = glfwGetTime();
                paused_ = false;
            }
        }
        void pause()
        {
            if (!paused_)
            {
                double now = glfwGetTime();
                cumulation_ += now-start_;
                paused_ = true;
            }
        }
    };
}
