#pragma once

#include <string>
#include "rgcp.h"

class ISimulator;

class Interpreter
{
public:
    Interpreter(ISimulator* sim) : sim_(sim) {}
    virtual std::string interpret(rgcp::request const&) = 0;

protected:
    ISimulator* simulator() { return sim_; }
private:
    ISimulator* sim_;
};

#define DECLARE_INTERPRETER_CLASS(CLASSNAME)    \
    class CLASSNAME : public Interpreter         \
    {                                                   \
    public:                                             \
        CLASSNAME(ISimulator* sim) : Interpreter(sim) {} \
        std::string interpret(rgcp::request const&);    \
    };                                                  \


DECLARE_INTERPRETER_CLASS(ErrorInterpreter);
DECLARE_INTERPRETER_CLASS(SnapInterpreter);
DECLARE_INTERPRETER_CLASS(ListInterpreter);
DECLARE_INTERPRETER_CLASS(SetVelInterpreter);
DECLARE_INTERPRETER_CLASS(GetPosInterpreter);
DECLARE_INTERPRETER_CLASS(StartDribblerInterpreter);
