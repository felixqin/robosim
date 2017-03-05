#include <iostream>
#include <cmath>
#include <stdio.h>
#include <exception>
#include <windows.h>
#include "../shared/ctrl_algo.h"
#include "../../rgcp/rgcp.hpp"

using namespace std;


// 发远请求并返回请求的响应
rgcp::response invoke(rgcp::client &client, rgcp::request const& r)
{
    std::string qs = to_string(r);
    DWORD t0 = GetTickCount();
    cout<<"> "<<qs<<endl;
    std::string ps = client.invoke(qs);
    cout<<ps<<endl;
    cout<<"Invoke time(ms): "<<GetTickCount()-t0<<endl;
    return ps;
}

void loop(char const* ip, int port, int interval = 300)
{
    rgcp::client client;
    client.open(ip,port);

    for(;;)
    {
        using namespace rgcp;

        //取球和蓝队0号队员的位置
        response pos_ball = invoke(client,request(command::get_pos,group::ball,0));
        response pos_blue = invoke(client,request(command::get_pos,group::blue,0));

        if (pos_ball.is_valid()&&pos_blue.is_valid())
        {
            //从响应包中解析位置
            double ball_x,ball_y,ball_theta;
            parse_x_y_theta(pos_ball.content(),ball_x,ball_y,ball_theta);

            //从响应包中解析位置
            double blue_x,blue_y,blue_theta;
            parse_x_y_theta(pos_blue.content(),blue_x,blue_y,blue_theta);

            //计算向量的角度
            double a = get_angle(ball_x-blue_x,ball_y-blue_y);

            //计算应该设定给球员的速度
            double vx,vy,w;
            gen_vel(blue_x,blue_y,blue_theta,
                    ball_x,ball_y,a,
                    0.5,0.8,
                    vx,vy,w);
            //设定球员速度
            invoke(client,request(command::set_vel,group::blue,0,vx,vy,w));
        }
        Sleep(interval);
    }
}

void loop2(char const* ip, int port, int interval = 300)
{
    rgcp::client client;
    client.open(ip,port);

    rgcp::id_type ball_id(rgcp::group::ball,0);
    rgcp::id_type blue_id(rgcp::group::blue,0);

    for(;;)
    {
        using namespace rgcp;

        //获取位置快照
        response snap_response = invoke(client,request(command::snap));

        //解释快照
        position_map_type pm;
        DWORD t0 = GetTickCount();
        parse_snap(snap_response.content(),pm);
        cout<<"parse_snap time(ms): "<<GetTickCount()-t0<<endl;

        if (pm.count(ball_id)&&pm.count(blue_id))
        {
            //取球和蓝队0号队员的位置
            position_type ball_pos = pm[ball_id];
            position_type blue_pos = pm[blue_id];

            //计算向量的角度
            double a = get_angle(ball_pos.x-blue_pos.x,ball_pos.y-blue_pos.y);

            //计算应该设定给球员的速度
            double vx,vy,w;
            gen_vel(blue_pos.x,blue_pos.y,blue_pos.theta,
                    ball_pos.x,ball_pos.y,a,
                    0.5,0.8,
                    vx,vy,w);
            //设定球员速度
            invoke(client,request(command::set_vel,group::blue,0,vx,vy,w));
        }
        Sleep(interval);
    }
}

void out_position(rgcp::position_map_type::value_type const& item)
{
    rgcp::id_type id = item.first;
    rgcp::position_type pos = item.second;
    std::cout<<rgcp::tag_to_string(id.first)<<' '<<id.second<<": "
            <<pos.x<<' '<<pos.y<<' '<<pos.theta<<'\n';
}

void test_snap(char const* ip, int port)
{
    rgcp::client client;
    client.open(ip,port);

    using namespace rgcp;

    response snap_response = invoke(client,request(command::snap));
    position_map_type pm;
    parse_snap(snap_response.content(),pm);
    cout<<pm.size()<<endl;

    std::for_each(pm.begin(),pm.end(),out_position);
}

int main(int argc, char* argv[])
{
    if (argc!=4)
    {
        cout<<"Usage: <ip> <port> <interval>";
        return 0;
    }

    try
    {
        //test_snap(argv[1],atoi(argv[2]));
        //loop(argv[1],atoi(argv[2]),atoi(argv[3]));
        loop2(argv[1],atoi(argv[2]),atoi(argv[3]));
    }
    catch (std::exception const& e)
    {
        cerr<<e.what()<<endl;
    }

    system("pause");

    return 0;
}
