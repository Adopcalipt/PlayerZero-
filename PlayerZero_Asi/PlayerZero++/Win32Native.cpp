#include "Win32Native.h"

namespace Win32Native
{
    LPVOID GetCurrentProcess()
    {
        return ::GetCurrentProcess();
    }

    HMODULE GetModuleHandle(LPCSTR lpModuleName)
    {
        return ::GetModuleHandleA(lpModuleName);
    }

    BOOL GetModuleInformation(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb)
    {
        return ::GetModuleInformation(hProcess, hModule, lpmodinfo, cb);
    }
}