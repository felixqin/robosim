#include <windows.h>
#include "platform/win32/api.h"

using namespace std;

namespace platform
{
    string GetAppPath()
    {
        char path[MAX_PATH] = {0};
        DWORD len = GetModuleFileName(0L,path,MAX_PATH);
        while ( len>0 && path[--len]!='\\' );
        path[len] = 0;
        return path;
    }

    bool SearchFiles(std::vector<string>& filelist, const char* dir, const char* extname)
    {
        string path = dir;
        if ( path[path.size()-1] != '\\' )
        {
            path += '\\';
        }

        string filename = path + "*" + extname;
        WIN32_FIND_DATA wfd;
        HANDLE hSearch = FindFirstFile(filename.c_str(),&wfd);
        if (hSearch != INVALID_HANDLE_VALUE)
        {
            do
            {
                filelist.push_back(path+wfd.cFileName);
            }
            while ( FindNextFile(hSearch,&wfd) );
            FindClose(hSearch);
            return true;
        }
        return false;
    }

}

