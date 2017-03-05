#pragma once

#include <unordered_map>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

template <typename charT>
class basic_ini_variables
{
public:
    typedef std::basic_string<charT> string_type;
    typedef std::tr1::unordered_map<string_type,string_type> variables_type;

public:
    basic_ini_variables(boost::program_options::basic_parsed_options<charT> const& options)
    {
        for (unsigned i=0; i<options.options.size(); ++i)
        {
            string_type key = boost::algorithm::to_lower_copy(options.options[i].string_key);
            vars_[key] = options.options[i].value.front();
        }
    }

    template<typename T>
    T get(string_type const& key, T const& default_value)
    {
        string_type k = boost::algorithm::to_lower_copy(key);
        if (vars_.count(k)==0)
        {
            vars_[k] = boost::lexical_cast<string_type>(default_value);
            return default_value;
        }
        try
        {
            return boost::lexical_cast<T>(vars_[k]);
        }
        catch (boost::bad_lexical_cast const& )
        {
            return default_value;
        }
    }
    template<typename T>
    void set(string_type const& key, T const& value)
    {
        string_type k = boost::algorithm::to_lower_copy(key);
        vars_[k] = boost::lexical_cast<string_type>(value);
    }

    variables_type const& variables() const { return vars_; }

private:
    variables_type vars_;

};

typedef basic_ini_variables<char> ini_variables;
typedef basic_ini_variables<wchar_t> wini_variables;


template<typename charT>
boost::program_options::basic_parsed_options<charT>
parse_ini_file(std::basic_istream<charT>& is)
{
    using namespace boost::program_options;
    options_description desc("");
    return parse_config_file(is,desc,true);
}
