#pragma once

#include <vector>
#include <robosim/types.h>

#include "robotparam.h"

class TeamConfig
{
public:
    TeamConfig(const char* ini_file)
    {
        loadConfig(ini_file);
    }

    size_t getRobotCount() const { return robot_count_; }
    const RobotParam* getRobotParam() const {return &robot_param_;}
    /*
    const float* getRobotVertices() const;
    size_t getRobotVerticesCount() const;
    const float* getRobotIndices() const;
    size_t getRobotIndicesCount() const;
    */
protected:
    void loadConfig(const char* ini_file);
private:
    size_t robot_count_;
    RobotParam robot_param_;
    std::vector<WheelParam> wheels_;

    std::vector<Real>           robot_vertices_;
    std::vector<unsigned int>   robot_indices_;
    std::vector<Real>           robot_normals_;
};

