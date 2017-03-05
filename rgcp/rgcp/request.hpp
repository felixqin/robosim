#pragma once

#include <sstream>
#include <boost/algorithm/string.hpp>
#include "tag.hpp"
#include "constants.h"
#include "parsers.hpp"

namespace rgcp
{
    class request
    {
    public: // ctor
        request(tag cmd=0,
                tag group=0,
                int number=0,
                double x=0,
                double y=0,
                double theta=0)
            : cmd_(cmd),group_(group),number_(number),x_(x),y_(y),theta_(theta),err_(0)
        {
        }
        template<typename StringT>
        request(StringT request_string)
            : cmd_(0),err_(0)
        {
            std::vector<std::string> tokens;
            boost::algorithm::split(tokens,request_string,boost::algorithm::is_space());
            if (!tokens.empty())
            {
                from_tokens_(tokens);
            }
        }
    public: // property
        tag command() const { return cmd_; }
        int error() const { return err_; }
        tag group() const { return group_; }
        int number() const { return number_; }
        double x() const { return x_; }
        double y() const { return y_; }
        double theta() const { return theta_; }

    public: // status
        bool is_valid() const { return cmd_ && !err_; }

    private:
        void from_tokens_(std::vector<std::string> const&);

    private:
        tag         cmd_;
        tag         group_;
        int         number_;
        double      x_,y_,theta_;
        int         err_;
    };

    inline
    void request::from_tokens_(std::vector<std::string> const& tokens)
    {
        cmd_ = tag_from_string(tokens[0]);
        switch(cmd_)
        {
        case command::snap:
            break;
        case command::list:
            err_ = parse_request(tokens,group_);
            break;
        case command::get_pos:
        case command::start_dribbler:
        case command::stop_dribbler:
            err_ = parse_request(tokens,group_,number_);
            break;
        case command::set_vel:
            err_ = parse_request(tokens,group_,number_,x_,y_,theta_);
            break;
        default:
            err_ = error_code::unknown_command;
        }
    }

    ////////////

    inline
    std::string to_string(request const& r)
    {
        if (r.is_valid())
        {
            std::stringstream ss;
            ss<<rgcp::tag_to_string(r.command());
            switch (r.command())
            {
            case command::snap:
                break;
            case command::list:
                ss<<' '<<rgcp::tag_to_string(r.group());
                break;
            case command::get_pos:
            case command::start_dribbler:
            case command::stop_dribbler:
                ss<<' '<<rgcp::tag_to_string(r.group());
                ss<<' '<<r.number();
                break;
            case command::set_vel:
                ss<<' '<<rgcp::tag_to_string(r.group());
                ss<<' '<<r.number();
                ss<<' '<<int(1000*r.x());
                ss<<' '<<int(1000*r.y());
                ss<<' '<<int(1000*r.theta());
                break;
            }
            ss<<"\r\n";
            return ss.str();
        }
        else
        {
            return "#INVALID_REQUEST#";
        }
    }

}
