#ifndef CONFIG_IMPL_H__
#define CONFIG_IMPL_H__

#include <exception>
#include <string>
#include <vector>
#include <tr1/memory>
#include "robosim/util/config.h"

namespace config {

class SectionImpl;
class ConfigImpl;
class IOImpl;

class ConfigImplException : public std::exception
{
};

/************************************************************************/

class VarImpl
{
    VarImpl(const VarImpl&);
    VarImpl& operator=(const VarImpl&);
public:
    VarImpl(SectionImpl* parent, int line)
        : parent_(parent), line_(line)
    {}

    int load(std::istream& in);

    bool empty() const { return name_.empty();}
    const std::string& name() const { return name_; }
    const std::string& value() const { return value_; }

    bool setValue(const char* value);

private:
    SectionImpl* parent_;

    std::string name_;
    std::string value_;

    int line_,value_pos_;
};

/*************************************************************************/

typedef std::vector<std::tr1::shared_ptr<VarImpl> > VarTable;

class SectionImpl
{
    SectionImpl(const SectionImpl&);
    SectionImpl& operator=(const SectionImpl&);
public:
    SectionImpl(ConfigImpl* config, int line_begin)
        : parent_(config),line_begin_(line_begin),line_num_(0)
    {}

    int load(std::istream&);

    bool empty() const { return vars_.empty(); }
    const std::string& name() const { return name_; }
    const VarTable& getVars() const { return vars_; }

    bool modifyVar(int varline, int value_begin, int value_end, const char* value);

private:
    ConfigImpl* parent_;

    std::string name_;
    VarTable vars_;

    int line_begin_,line_num_;
};

/**************************************************************************/

typedef std::vector<std::tr1::shared_ptr<SectionImpl> > SectionTable;

class ConfigImpl
{
    ConfigImpl(const ConfigImpl& );
    ConfigImpl& operator=(const ConfigImpl&);
public:
    ConfigImpl(IOImpl* io) : io_(io),line_num_(0) {}

    int load(std::istream& in);

    bool empty() const { return sections_.empty(); }
    const SectionTable& getSections() const { return sections_;}

    bool modifyVar(int line, int pos_begin, int pos_end, const char* value);

private:
    IOImpl* io_;

    SectionTable sections_;

    int line_num_;
};

/**************************************************************************/

class IOImpl
{
    IOImpl(const IOImpl&);
    IOImpl& operator=(const IOImpl&);
public:
    IOImpl(const char* filename);
    ConfigImpl& getConfig() { return config_;}

    void read();
    bool modifyLine(int line, int pos_begin, int pos_end, const char* value);

private:
    std::string filename_;
    ConfigImpl config_;
};

/**************************************************************************/
inline
bool VarImpl::setValue(const char* value )
{
    if (value_ == value ) return false;

    if (parent_->modifyVar(line_,value_pos_,value_pos_+value_.size(),value))
    {
        value_ = value;
        return true;
    }
    return false;
}

inline
bool SectionImpl::modifyVar(int varline, int value_begin, int value_end, const char* value)
{
    return parent_->modifyVar(line_begin_+varline, value_begin, value_end, value);
}

inline
bool ConfigImpl::modifyVar(int line, int pos_begin, int pos_end, const char* value)
{
    return io_->modifyLine(line, pos_begin, pos_end, value);
}



}   //namespace config
#endif

