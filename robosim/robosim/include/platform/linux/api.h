#pragma once

#include <vector>
#include <string>

namespace platform
{
    std::string GetAppPath();
    bool SearchFiles(std::vector<std::string>& filelist,const char* dir, const char* extname);
}

