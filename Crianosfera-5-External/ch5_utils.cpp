#include "CH5_External.h"

/*void DbgPrint(LPCSTR form, ...)
{
	static char tmp[2048];
	va_list p;
	va_start(p, form);
	vsprintf(tmp, form, p);
	MessageBox(0, tmp, "Information", MB_ICONINFORMATION);
	va_end(p);
}*/

DWORD CH5::Utils::GetModuleAddress(DWORD dwProcessID, const char* szModuleName)
{
	DWORD dwResult = 0;
	DWORD dwBytesNeeded = 0;
	HMODULE hModulePointers[1024];
	char szModule[MAX_PATH];
	HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hGameHandle != INVALID_HANDLE_VALUE)
	{
		if (EnumProcessModules(hGameHandle, hModulePointers, sizeof(hModulePointers), &dwBytesNeeded))
		{
			for (unsigned int i = 0; i < (dwBytesNeeded / sizeof(HMODULE)); i++)
			{
				if (GetModuleFileNameEx(hGameHandle, hModulePointers[i], szModule, sizeof(szModule) / sizeof(char)))
				{
					if (strstr(szModule, szModuleName))
					{
						dwResult = (DWORD)hModulePointers[i];
						break;
					}
				}
			}
		}
		CloseHandle(hGameHandle);
	}
	return dwResult;
	/*
	DWORD_PTR dwModuleBaseAddress = 0;
	DWORD result = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry32;
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &ModuleEntry32))
		{
			do
			{
				if (strstr(ModuleEntry32.szModule, szModuleName) == 0)
				{
					dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr;
					result = dwModuleBaseAddress;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);
	}
	return result;
	*/
}