#pragma once

#include <stdexcept>
#include <dlfcn.h>

namespace platform
{
    class DynamicLibrary
    {
					DynamicLibrary(const DynamicLibrary&);
					DynamicLibrary& operator=(const DynamicLibrary&);
    public:
        typedef void* HandleType;
        typedef void* ProcAddressType;

        DynamicLibrary() : handle_(0) {}
        DynamicLibrary(const char* filename)
        {
            open(filename);
        }
        virtual ~DynamicLibrary()
        {
            close();
        }

        void open(const char* filename)
        {
            handle_ = dlopen(filename,RTLD_NOW);
        }
        void close()
        {
            if (is_open()) dlclose(handle_);
        }
        ProcAddressType getProcAddress(const char* procName)
        {
            return is_open() ? dlsym(handle_,procName) : 0;
        }
        bool is_open()
        {
            return handle_!=0;
        }
    private:
        HandleType handle_;
    };
}

