#pragma once


#include <string>
#include <vector>
#include "arg_parser.hpp"

using namespace std;

namespace rgcp {

/******************************************************************************/
inline
int parse_request(vector<string> const& tokens,tag& group)
{
    if (tokens.size()<2)
    {
        return error_code::command_format;
    }
    group = tag_from_string(tokens[1]);
    return error_code::null;
}
inline
int parse_request(vector<string> const& tokens, tag& group, int& number)
{
    if (tokens.size()<3)
    {
        return error_code::command_format;
    }
    return parse_group_number(&tokens[1],&tokens[3],group,number);
}

inline
int parse_request(vector<string> const& tokens, tag& group, int& number, double& x, double& y, double& theta)
{
    if (tokens.size()<6)
    {
        return error_code::command_format;
    }
    parse_group_number(&tokens[1],&tokens[3],group,number);
    parse_x_y_theta(&tokens[3],&tokens[6],x,y,theta);
    return error_code::null;
}

}
