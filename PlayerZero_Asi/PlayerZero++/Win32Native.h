#ifndef WIN32NATIVE_H
#define WIN32NATIVE_H

#include <Windows.h>
#include <psapi.h>

namespace Win32Native
{
    extern "C" {
        typedef struct MODULEINFO {
            LPVOID lpBaseOfDll;
            DWORD SizeOfImage;
            LPVOID EntryPoint;
        } MODULEINFO;

        LPVOID GetCurrentProcess();
        HMODULE GetModuleHandle(LPCSTR lpModuleName);
        BOOL GetModuleInformation(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);
    }
}

#endif // WIN32NATIVE_H