#pragma once

#include "tag.hpp"

namespace rgcp
{

struct command
{
    enum { null     = 0
        ,snap       = to_tag<'S','N','A','P'>::value
        ,list       = to_tag<'L','I','S','T'>::value
        ,get_pos    = to_tag<'G','P','O','S'>::value
        ,set_vel    = to_tag<'S','V','E','L'>::value
        ,start_dribbler = to_tag<'S','D','R','I'>::value
        ,stop_dribbler  = to_tag<'P','D','R','I'>::value
    };
};

struct group
{
    enum { null = 0
        ,ball   = to_tag<'B','A','L','L'>::value
        ,blue   = to_tag<'B','L','U','E'>::value
        ,yellow = to_tag<'Y','L','L','W'>::value
    };
};

struct error_code
{
    enum { null = 0
        ,unknown = 1            //未知错误
        ,unknown_command = 101  //未知命令
        ,command_format         //命令格式错误
        ,too_many_args
        ,unknown_object = 201   //对象或组不存在
    };
};


}
