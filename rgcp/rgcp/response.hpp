#pragma once

#include <sstream>
#include <boost/algorithm/string.hpp>
#include "tag.hpp"
#include "constants.h"

namespace rgcp
{
    class response_header
    {
    public: // ctor
        response_header(int err=0,tag cmd=0)
            : err_(err),cmd_(cmd)
        {
        }
        response_header(std::string err,std::string cmd)
        {
            if (err.empty()) return;

            if (err[0]=='S'&&err[1]=='\0')
            {
                err_=0;
            }
            else if (err[0]=='F')
            {
                err_=std::atoi(&err[1]);
            }
            cmd_ = tag_from_string(cmd);
        }
    public: // property
        tag command() const { return cmd_; }
        int error() const { return err_; }

    public: // status
        bool is_valid() const { return cmd_ && !err_; }

    private:
        int err_;
        tag cmd_;
    };

    ////////////
    inline
    std::string to_string(response_header const& rh)
    {
        std::stringstream ss;
        if (rh.error())
        {
            ss<<'F'<<rh.error();
        }
        else
        {
            ss<<'S';
        }
        ss<<' '<<tag_to_string(rh.command());
        return ss.str();
    }

    ////////////////
    class response
    {
    public: // types
        typedef std::vector<std::string> content_type;
    public: // ctor
        response() {}
        template<typename StringT>
        response(StringT str)
        {
            std::vector<std::string> tokens;
            boost::algorithm::split(tokens,str,boost::algorithm::is_space());
            if (tokens.size()>=2)
            {
                header_ = response_header(tokens[0],tokens[1]);
                std::copy(&tokens[2],&tokens[0]+tokens.size(),std::back_inserter(content_));
            }
        }
    public: //property
        response_header const& header() const { return header_; }
        content_type const& content() const { return content_; }

    public: //status
        bool is_valid() const { return header_.is_valid();}

    private:
        response_header header_;
        content_type    content_;
    };

    ////////////////

    inline
    std::string to_string(response const& r)
    {
        std::stringstream ss;
        ss<<to_string(r.header());
        for(response::content_type::const_iterator first = r.content().begin(), last = r.content().end();
            first!=last;
            ++first)
        {
            ss<<' '<<*first;
        }
        ss<<"\r\n";
        return ss.str();
    }
}

