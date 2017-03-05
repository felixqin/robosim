#pragma once

#include <boost/range.hpp>
#include "../tag.hpp"

using namespace std;

namespace rgcp
{

template<typename TokensIt>
int parse_group_number(TokensIt first, TokensIt last, tag& group, int& number)
{
    if (std::distance(first,last)<2)
    {
        return error_code::command_format;
    }

    group = tag_from_string(first->c_str());
    number = std::atoi((++first)->c_str());
    return error_code::null;
}

template<typename TokenSequence>
int parse_group_number(TokenSequence const& tokens, tag& group, int& number)
{
    return parse_group_number(boost::begin(tokens),boost::end(tokens),group,number);
}

template<typename TokensIt>
int parse_x_y_theta(TokensIt first, TokensIt last, double &x, double &y, double &theta)
{
    if (std::distance(first,last)<3)
    {
        return error_code::command_format;
    }

    x = 0.001f*std::atoi(first->c_str());
    y = 0.001f*std::atoi((++first)->c_str());
    theta = 0.001f*std::atoi((++first)->c_str());
    return error_code::null;
}

template<typename TokenSequence>
int parse_x_y_theta(TokenSequence const& tokens, double &x, double &y, double &theta)
{
    return parse_x_y_theta(boost::begin(tokens),boost::end(tokens),x,y,theta);
}

}

