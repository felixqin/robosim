#ifndef DLL_H__
#define DLL_H__

#include <windows.h>

namespace platform
{

    class DynamicLibrary
    {
        DynamicLibrary(const DynamicLibrary&);
        DynamicLibrary& operator=(const DynamicLibrary&);
    public:
        typedef HINSTANCE  HandleType;
        typedef FARPROC SymbolType;
        //typedef int (__stdcall *SymbolType)();

        DynamicLibrary() : handle_(0) {}
        DynamicLibrary(const char* filename)
            : handle_(0)
        {
            open(filename);
        }
        virtual ~DynamicLibrary()
        {
            close();
        }

        void open(const char* filename)
        {
            handle_ = LoadLibrary(filename);
        }
        void close()
        {
            if (is_open()) FreeLibrary(handle_);
        }
        bool is_open()
        {
            return handle_!=0;
        }
        SymbolType getSymbol(const char* procName)
        {
            return is_open() ? GetProcAddress(handle_,procName) : 0;
        }
    private:
        HandleType handle_;
    };
}

#endif
