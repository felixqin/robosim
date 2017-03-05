#ifndef TEAM_H__
#define TEAM_H__

#include <vector>
#include <memory>

class Robot;
class TeamConfig;

class RobotTeam
{
public:
    RobotTeam(ISimulator* sim, Tag group, const TeamConfig* cfg);
    ~RobotTeam();

private:
    typedef std::tr1::shared_ptr<Robot> RobotSP;
    std::vector<RobotSP> robotv_;
};


#endif
