#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <robosim/base.h>
#include "interpret.h"


class Dispatcher
{
    typedef std::tr1::shared_ptr<Interpreter> InterpreterPtr;

public:
    Dispatcher(ISimulator* sim)
    {
        if (!sim) return;

        interpreters_[0] = InterpreterPtr(new ErrorInterpreter(sim));
        interpreters_[rgcp::command::snap] = InterpreterPtr(new SnapInterpreter(sim));
        interpreters_[rgcp::command::list] = InterpreterPtr(new ListInterpreter(sim));
        interpreters_[rgcp::command::get_pos] = InterpreterPtr(new GetPosInterpreter(sim));
        interpreters_[rgcp::command::set_vel] = InterpreterPtr(new SetVelInterpreter(sim));
        interpreters_[rgcp::command::start_dribbler] = InterpreterPtr(new StartDribblerInterpreter(sim));
    }
    std::string dispatch(std::string const& request)
    {
        assert(!interpreters_.empty());

        rgcp::request r(request);

        if (!r.is_valid())
        {
            return interpreters_[0]->interpret(r);
        }

        if (interpreters_.count(r.command()))
        {
            return interpreters_[r.command()]->interpret(r);
        }

        return std::string();
    }

private:
    std::tr1::unordered_map<Tag,InterpreterPtr> interpreters_;
};

