#include <fstream>
#include "config_impl.h"

namespace config
{

using namespace std;

template<class InIt>
InIt skip_space(InIt first, InIt last)
{
    InIt p = first;
    while ( p!=last && isspace(*p) ) ++p;
    return p;
}
//是否是无效行
template<class InIt>
bool is_invalid_line(InIt first, InIt last)
{
    InIt p = skip_space(first,last);
    return p == last || *p == '#' || *p == '\n' || *p == 0;
}
string get_line(istream&in)
{
    static const int MAX_LINE = 256;
    char buf[MAX_LINE];
    in.getline(buf,MAX_LINE);
    return buf;
}
//跳过无效行(注释行或空行),返回跳过的行数
int skip_invalid_line(istream& in)
{
    int n = 0;
    istream::pos_type pos = in.tellg();
    while(!in.eof())
    {
        string line = get_line(in);
        if (!is_invalid_line(line.begin(),line.end()))
        {
            in.clear();
            in.seekg(pos);
            break;
        }
        ++n;
        pos = in.tellg();
    }
    return n;
}
template<class InIt>
string get_section_name(InIt first, InIt last)
{
    string name;
    InIt p = skip_space(first,last);
    if ( p != last && *p == '[' )
    {
        ++p;
        while ( p!= last && *p != ']' )
        {
            name.push_back(*p);
            ++p;
        }
    }
    return name;
}
/******************************************************************/
IOImpl::IOImpl(const char* filename)
    :
    filename_(filename),
    config_(this)
{
}
void IOImpl::read()
{
    ifstream in(filename_.c_str(),ios_base::binary);
    config_.load(in);
}
bool IOImpl::modifyLine(int line, int pos_begin, int pos_end, const char* value)
{
    ifstream in(filename_.c_str());
    if ( !in.is_open() ) return false;

    vector<string> lines;
    int line_num = 0;
    while ( !in.eof() )
    {
        if( line_num == line )
        {
            string line = get_line(in);
            line.replace(pos_begin,pos_end-pos_begin,value);
            lines.push_back(line);
        }
        else
        {
            lines.push_back(get_line(in));
        }
        ++line_num;
    }
    in.close();

    ofstream out(filename_.c_str());
    if ( !out.is_open() ) return false;

    vector<string>::const_iterator it;
    for ( it = lines.begin(); it != lines.end(); ++it )
    {
        out<<*it<<endl;
    }
    return true;
}

/******************************************************************/
int ConfigImpl::load(std::istream& in)
{
    line_num_ = 0;
    while (!in.eof())
    {
        line_num_ += skip_invalid_line(in);

        tr1::shared_ptr<SectionImpl> section
                = tr1::shared_ptr<SectionImpl>(new SectionImpl(this,line_num_));
        line_num_ += section->load(in);
        sections_.push_back(section);
    }
    return line_num_;
}
/******************************************************************/
int SectionImpl::load(std::istream& in)
{
    line_num_ = 0;
    string line = get_line(in);
    ++line_num_;
    name_ = get_section_name(line.begin(),line.end());

    if (name_.empty()) throw ConfigImplException();

    while (!in.eof())
    {
        line_num_ += skip_invalid_line(in);

        tr1::shared_ptr<VarImpl> var
                = tr1::shared_ptr<VarImpl>(new VarImpl(this,line_num_));
        line_num_ += var->load(in);

        if (var->empty()) break;

        vars_.push_back(var);
    }
    return line_num_;
}

/******************************************************************************/
int VarImpl::load(std::istream& in)
{
    if (in.eof()) return 0;

    istream::pos_type pos = in.tellg();

    string line = get_line(in);

    string::const_iterator first = line.begin();
    string::const_iterator last = line.end();
    string::const_iterator p = skip_space(first,last);
    if ( *p == '[' )
    {
        in.clear();
        in.seekg(pos);
        return 0;
    }

    while ( p!= last && !isspace(*p) && *p != '=' && *p!= '#' )
    {
        name_.push_back(*p);
        ++p;
    }
    p = skip_space(p,last);
    if ( *p != '=' ) throw ConfigImplException(); //缺少等号

    ++p;
    p = skip_space(p,last);
    value_pos_ = p-first;
    while ( p!= last && !isspace(*p) && *p!='#' )
    {
        value_.push_back(*p);
        ++p;
    }

    return 1;
}

}
