#ifndef PARAMETERS_H__
#define PARAMETERS_H__

#ifndef MAX_PATH
#define MAX_PATH    260
#endif

struct SystemParameters
{
    char app_path[MAX_PATH];
    char install_path[MAX_PATH];
    char plugin_path[MAX_PATH];
    char res_path[MAX_PATH];

    float frame_time;
    float sim_step_time;
};

struct ModelParameters
{
    float gravity;
    float cfm;
    float erp;
    float bounce;
    float bounce_vel;

    float max_step_time;
    int max_contacts;
};


#endif //PARAMETERS_H__
