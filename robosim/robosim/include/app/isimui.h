#pragma once

class IDrawing;

class ISimUI
{
public:
    virtual ~ISimUI() {}

    virtual bool open(int w,int h) = 0;
    virtual bool closed() = 0;
    virtual void sleep(double sec) = 0;
    virtual IDrawing* beginPaint() = 0;
    virtual void endPaint() = 0;
};
