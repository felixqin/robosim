#include <sstream>
#include <iterator>
#include <robosim/robosim.h>
#include <robosim/util/odeext.h>
#include "interpret.h"

using namespace rgcp;


inline
int milli_real_to_int(float r)
{
    return int(r*1000);
}

/******************************************************************************/

inline
std::ostream& output_x_y_theta(std::ostream& os,float x, float y, float theta)
{
    os<<milli_real_to_int(x)<<' '
      <<milli_real_to_int(y)<<' '
      <<milli_real_to_int(theta);
    return os;
}

inline
std::ostream& output_success_header(std::ostream& os,rgcp::request const& request)
{
    os<<"S "<<rgcp::tag_to_string(request.command())<<' ';
    return os;
}
std::ostream& output_group_status(std::ostream& out, ISimulatorStatus const* ss, Tag group)
{
    enum {max_size=10};
    int list[max_size];
    size_t count = ss->getGroupMember(group,list,max_size);
    if (count)
    {
        out<<rgcp::tag_to_string(group)<<" [ ";
        for (size_t i=0;i<count;++i)
        {
            EntityStatus es;
            if (ss->getEntityStatus(group,list[i],es))
            {
                float theta = XYThetaForRotation(es.qw,es.qx,es.qy,es.qz);
                out<<list[i]<<' ';
                output_x_y_theta(out,es.px,es.py,theta);
                out<<' ';
            }
        }
        out<<"] ";
    }
    return out;
}

/******************************************************************************/

inline
std::string error_response(Tag cmd, int err)
{
    std::ostringstream ss;
    ss<<"F";
    ss.width(3);
    ss.fill('0');
    ss<<err<<' '<<rgcp::tag_to_string(cmd)<<"\r\n";
    return ss.str();
}

std::string only_post_msg_response(rgcp::request const& request, ISimulator* sim, MessageID msg)
{
    try
    {
        const IMessageHandler* handler = sim->getStatus()->getMsgHandler(request.group(),request.number());
        if(!handler)
        {
            return error_response(request.command(),error_code::unknown_object);
        }
        sim->getCommunicator()->post(handler,msg);

        std::ostringstream ss;
        output_success_header(ss,request);
        return ss.str();
    }
    catch (const std::exception& e)
    {
        return error_response(request.command(),error_code::unknown);
    }
}

/******************************************************************************/

std::string ErrorInterpreter::interpret(rgcp::request const& request)
{
    return error_response(request.command(),request.error());
}

std::string SnapInterpreter::interpret(rgcp::request const& request)
{
    ISimulatorStatus const* status = simulator()->getStatus();
    std::stringstream ss;
    output_success_header(ss,request);
    output_group_status(ss,status,rgcp::group::ball);
    output_group_status(ss,status,rgcp::group::blue);
    output_group_status(ss,status,rgcp::group::yellow);
    ss<<"\r\n";
    return ss.str();
}

std::string ListInterpreter::interpret(rgcp::request const& request)
{
    enum {max_size=10};
    int list[max_size];
    size_t count = simulator()->getStatus()->getGroupMember(request.group(),list,max_size);
    if (count==0)
    {
        return error_response(request.command(),rgcp::error_code::unknown_object);
    }

    std::ostringstream ss;
    output_success_header(ss,request);
    std::copy(&list[0],&list[count],std::ostream_iterator<int>(ss," "));
    ss<<"\r\n";
    return ss.str();
}

std::string StartDribblerInterpreter::interpret(rgcp::request const& request)
{
    bool action = true;
    MessageID msg = CreateMsg(MSG_SET_DRIBBLER,(char*)&action,sizeof(action));
    return only_post_msg_response(request,simulator(),msg);
}
std::string SetVelInterpreter::interpret(rgcp::request const& request)
{
    MessageID msg = CreateVelMsg(MSG_SET_VELOCITY,request.x(),request.y(),request.theta());
    return only_post_msg_response(request,simulator(),msg);
}

std::string GetPosInterpreter::interpret(rgcp::request const& request)
{
    EntityStatus status;
    bool existed = simulator()->getStatus()->getEntityStatus(request.group(),request.number(),status);
    if (!existed)
    {
        return error_response(request.command(),error_code::unknown_object);
    }

    float theta = XYThetaForRotation(status.qw,status.qx,status.qy,status.qz);

    std::ostringstream ss;
    output_success_header(ss,request);
    output_x_y_theta(ss,status.px,status.py,theta);
    ss<<"\r\n";
    return ss.str();
}
