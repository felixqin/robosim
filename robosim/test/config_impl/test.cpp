#include <iostream>
#include <fstream>
#include "config_impl.h"
#include "util/config.h"

using namespace std;
using namespace config;

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
int skip_invalid_line(istream& in);

/*******************************************************************************/
void test_skip_space()
{
    cout<<"test_skip_space():"<<endl;
    string str = " \t hello  ";
    string::iterator p = skip_space(str.begin(),str.end());
    cout<<p-str.begin()<<endl;
    cout<<&*p<<endl;
}
void test_invalid_line()
{
    cout<<"test_invalid_line():"<<endl;
    string str = " sfd      \t  #hello";
    bool valid = !is_invalid_line(str.begin(),str.end());
    cout<<valid<<endl;
}
void test_seek()
{
    cout<<"test_seek():"<<endl;
    char ch;
    streampos pos;
    std::ifstream in("test.ini",ios_base::binary);

    //in.getpos();
//*
    pos = in.tellg();
    cout<<in.tellg();
    in>>ch;
    cout<<ch;

    cout<<in.tellg();
    cout<<in.tellg();
    in>>ch;
    cout<<ch;

    cout<<in.tellg();
    in>>ch;
    cout<<ch;

    cout<<in.tellg();
    in>>ch;
    cout<<ch;

    in.seekg(pos);
    cout<<in.tellg();
    in>>ch;
    cout<<ch;
/*/
    string line = get_line(in);
    //cout<<line;
    cout<<in.tellg();

    in>>ch;
    cout<<ch;
//*/
    pos = in.tellg();
    cout<<pos;
    in>>ch;
    cout<<ch;
    while(!in.eof()) in>>ch;

    cout<<"\neof:"<<endl;
    cout<<in.eof()<<in.fail()<<endl;
    in.clear();
    in.seekg(pos);
    cout<<"pos:"<<pos<<","<<in.tellg()<<endl;
    in>>ch;
    cout<<ch;
    in.clear();
    cout<<in.eof()<<endl;

    cout<<endl;
}

void test_config()
{
    IOImpl io("test.ini");
    io.read();

    const SectionTable& sections = io.getConfig().getSections();
    cout<<"section number:"<<sections.size()<<endl;
    for ( size_t i=0; i<sections.size(); i++ )
    {
        const SectionImpl& section = *sections[i];
        cout<<i<<":\t"<<section.name()<<endl;
        const VarTable& vars = section.getVars();
        for ( size_t j=0; j<vars.size(); j++ )
        {
            const VarImpl& var = *vars[j];
            cout<<"\t"<<j<<":\t"<<var.name()
                <<"\t=\t"<<var.value()
                <<endl;
        }
    }
}
void output_config(const Config& conf)
{
}

void test_cxx()
{
    class a
    {
    public:
        virtual void print() const { cout<<"a"; }
    };
    class b : public a
    {
    public:
        virtual void print() const { cout<<"b"; }
    };
    b bb;
    const a& aa = bb;
    const a aaa = bb;
    aa.print();
    bb.print();
    aaa.print();
}
int main()
{
    cout << "Hello world!" << endl;
    test_skip_space();
    test_invalid_line();
    test_seek();

    test_config();

    Config conf("test.ini");

    cout<<conf["hello"]["s"]<<endl;

    conf["hello"]["s"] = "test";
    cout<<conf["hello"]["s"]<<endl;

    test_config();

    test_cxx();

    return 0;
}
