/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#include "..\inc\main.h"
#include "script.h"
#include "keyboard.h"

#include <Windows.h>
#include <Psapi.h>

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		//g_MainModule = GetModuleHandle(NULL);
		//GetModuleInformation(GetCurrentProcess(), g_MainModule, &g_MainModuleInfo, sizeof(g_MainModuleInfo));
		
		scriptRegister(hInstance, ScriptMain);
		scriptRegisterAdditionalThread(hInstance, AddChaterBox);
		keyboardHandlerRegister(OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		break;
	}		
	return TRUE;
}