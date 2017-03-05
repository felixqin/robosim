#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include "platform/linux/api.h"

using namespace std;

namespace platform
{
    string GetAppPath()
    {
        char path[BUFSIZ] = "\0";
        int len = readlink ("/proc/self/exe", path, BUFSIZ);
        while ( len>0 && path[--len]!='/' );
        path[len] = 0;
        return path;
    }

    bool SearchFiles(std::vector<string>& filelist, const char* dir, const char* extname)
    {
        string path = dir;
        if ( path[path.size()-1] != '/' )
        {
            path += '/';
        }

        DIR *dirp;
        if ( (dirp = opendir(dir)) == NULL)
        {
            //throw std::runtime_error("Open Directory Error:\n");
            return false;
        }

        dirent *direntp;
        while ( (direntp = readdir(dirp) ) !=NULL )
        {
            if ( !strcmp(direntp->d_name, ".")
                || !strcmp(direntp->d_name, "..")
                || !strstr(direntp->d_name,extname) )
            {
                continue;
            }

            string filename = path+direntp->d_name;

            struct stat statbuf;
            if (stat(filename.c_str(), &statbuf) == -1)
            {
                //throw std::runtime_error("Get stat Error");
                return false;
            }
            if (!S_ISDIR(statbuf.st_mode))
            {
                filelist.push_back(filename);
            }

        }
        closedir(dirp);

        return true;
    }

}


