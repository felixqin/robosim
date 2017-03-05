#include <iostream>
#include <cmath>
#include <stdio.h>
#include <exception>
#include <windows.h>
#include "../shared/ctrl_algo.h"
//#include "rgcp_client.hpp"
#include "../../rgcp/rgcp.hpp"

using namespace std;

rgcp::response invoke(rgcp::client &clt, rgcp::request const& r)
{
    std::string qs = to_string(r);
    cout<<"> "<<qs<<endl;
    std::string ps = clt.invoke(qs);
    cout<<ps<<endl;
    return ps;
}

int main()
{
    cout << "Hello world!" << endl;
/*
    typedef float real;

    real x=0,y=-1;
    real tan_theta = y/x;
    real theta = atan(tan_theta);

    printf("%f\n",tan_theta);
    cout<<tan_theta<<endl;

    cout<<theta/M_PI*180<<endl;

    double vx,vy;
    vel_depart(0.0,0.0000000000001,10,vx,vy);
    cout<<vx<<' '<<vy<<endl;
    */
    double x0=sqrt(3),y0=1,theta0=M_PI/180*30;
    double x1=sqrt(3),y1=3,theta1=M_PI/180*90;

    //double x,y,theta;
    //coord_trans(x0,y0,theta0,x1,y1,theta1,x,y,theta);
    //cout<<x<<' '<<y<<' '<<theta/M_PI*180<<endl;

    double maxv=10,maxw=5;
    double vx,vy,w;
    gen_vel(x0,y0,theta0,x1,y1,theta1,maxv,maxw,vx,vy,w);
    cout<<vx<<' '<<vy<<' '<<w<<endl;

    rgcp::client clt;

    try
    {
        clt.open("127.0.0.1",8101);
        for(;;)
        {
            using namespace rgcp;

            request q(command::get_pos,group::blue,0);
            response p=invoke(clt,q);

            double x,y,theta;
            get_x_y_theta(p.content(),x,y,theta);

            double vx,vy,w;
            gen_vel(x,y,theta,0,0,-M_PI_2,0.2,0.2,vx,vy,w);

            request q_sp(command::set_vel,group::blue,0,vx,vy,w);

            invoke(clt,q_sp);

            Sleep(200);
        }
    }
    catch (std::exception const& e)
    {
        cerr<<e.what()<<endl;
    }

    system("pause");

    return 0;
}
