#include "CH5_External.h"

HWND wndHwnd;
RECT wndRect;
float gWorldToScreen[16];

bool ScreenTransform(Vector vPoint, float* vScreen)
{
    vScreen[0] = gWorldToScreen[0] * vPoint.x + gWorldToScreen[4] * vPoint.y + gWorldToScreen[8] * vPoint.z + gWorldToScreen[12];
    vScreen[1] = gWorldToScreen[1] * vPoint.x + gWorldToScreen[5] * vPoint.y + gWorldToScreen[9] * vPoint.z + gWorldToScreen[13];
    float z = gWorldToScreen[3] * vPoint.x + gWorldToScreen[7] * vPoint.y + gWorldToScreen[11] * vPoint.z + gWorldToScreen[15];

    if (z == 0.0f)
    {
        return 0.0f >= z;
    }
    else
    {
        vScreen[0] = vScreen[0] * (1.0f / z);
        vScreen[1] = vScreen[1] * (1.0f / z);
        return 0.0 >= (1.0f / z);
    }
}

int CH5::Engine::Width = 640;
int CH5::Engine::Height = 480;

bool CH5::Engine::WorldToScreen(Vector vPoint, float* vScreen)
{
    bool iResult = ScreenTransform(vPoint, vScreen);
    if (vScreen[0] < 1 && vScreen[1] < 1 && vScreen[0] > -1 && vScreen[1] > -1 && !iResult)
    {
        vScreen[0] = vScreen[0] * ((wndRect.right - wndRect.left) / 2) + ((wndRect.right - wndRect.left) / 2);
        vScreen[1] = -vScreen[1] * ((wndRect.bottom - wndRect.top) / 2) + ((wndRect.bottom - wndRect.top) / 2);
        return true;
    }
    return false;
}

bool CH5::Engine::GetCvarFloat(const char* szCvarName, float *pfValue)
{
    cvar_t currentCvar;
    char szVarName[60];
    if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwGetFirstCvar)
    {
        HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
        if (hGameHandle != INVALID_HANDLE_VALUE)
        {
            SIZE_T uiBytesRead = 0;
            DWORD dwAddressToRead = CH5::Offsets::dwHwDllBase + CH5::Offsets::dwGetFirstCvar;
            ReadProcessMemory(hGameHandle, (LPCVOID)dwAddressToRead, (LPVOID)&dwAddressToRead, sizeof(dwAddressToRead), &uiBytesRead);
            if (dwAddressToRead)
            {
                do
                {
                    ReadProcessMemory(hGameHandle, (LPCVOID)dwAddressToRead, (LPVOID)&currentCvar, sizeof(currentCvar), &uiBytesRead);
                    if (currentCvar.name)
                    {
                        ReadProcessMemory(hGameHandle, (LPCVOID)currentCvar.name, (LPVOID)szVarName, sizeof(szVarName), &uiBytesRead);
                        szVarName[sizeof(szVarName) - 1] = 0;
                        if (!strncmp(szCvarName, szVarName, sizeof(szVarName)))
                        {
                            *pfValue = currentCvar.value;
                            CloseHandle(hGameHandle);
                            return true;
                        }
                    }
                    dwAddressToRead = (DWORD)currentCvar.next;
                } while (currentCvar.next);
            }
            CloseHandle(hGameHandle);
        }
    }
    return false;
}

bool CH5::Engine::SetCvarFloat(const char* szCvarName, float fValue)
{
    cvar_t currentCvar;
    char szVarName[60];
    if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwGetFirstCvar)
    {
        HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
        if (hGameHandle != INVALID_HANDLE_VALUE)
        {
            SIZE_T uiBytesRead = 0;
            DWORD dwAddressToRead = CH5::Offsets::dwHwDllBase + CH5::Offsets::dwGetFirstCvar;
            ReadProcessMemory(hGameHandle, (LPCVOID)dwAddressToRead, (LPVOID)&dwAddressToRead, sizeof(dwAddressToRead), &uiBytesRead);
            if (dwAddressToRead)
            {
                do
                {
                    ReadProcessMemory(hGameHandle, (LPCVOID)dwAddressToRead, (LPVOID)&currentCvar, sizeof(currentCvar), &uiBytesRead);
                    if (currentCvar.name)
                    {
                        ReadProcessMemory(hGameHandle, (LPCVOID)currentCvar.name, (LPVOID)szVarName, sizeof(szVarName), &uiBytesRead);
                        szVarName[sizeof(szVarName) - 1] = 0;
                        if (!strncmp(szCvarName, szVarName, sizeof(szVarName)))
                        {
                            currentCvar.value = fValue;
                            WriteProcessMemory(hGameHandle, (LPVOID)dwAddressToRead, (LPCVOID)&currentCvar, sizeof(currentCvar), &uiBytesRead);
                            CloseHandle(hGameHandle);
                            return true;
                        }
                    }
                    dwAddressToRead = (DWORD)currentCvar.next;
                } while (currentCvar.next);
            }
            CloseHandle(hGameHandle);
        }
    }
    return false;
}

void CH5::Engine::RePositionWindow()
{
    wndHwnd = FindWindow("Valve001", NULL);
    if (wndHwnd)
    {
        GetWindowRect(wndHwnd, &wndRect);
        Width = wndRect.right - wndRect.left;
        Height = wndRect.bottom - wndRect.top;
        MoveWindow(g_HackWnd, wndRect.left, wndRect.top/* + yoffset*/, Width, Height, true);
    }
}

void CH5::Engine::ReadViewMatrix()
{
    if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwWorldToScreen)
    {
        HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
        if (hGameHandle != INVALID_HANDLE_VALUE)
        {
            SIZE_T uiBytesRead = 0;
            ReadProcessMemory(hGameHandle, (LPCVOID)(CH5::Offsets::dwHwDllBase + CH5::Offsets::dwWorldToScreen), (LPVOID)&gWorldToScreen, sizeof(gWorldToScreen), &uiBytesRead);
            CloseHandle(hGameHandle);
        }
    }
}

bool CH5::Engine::IsGameActive()
{
    bool bFoundWindow = FindWindow("Valve001", NULL) != NULL;
    /*bool bConnected = false;
    if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwIsConnected)
    {
        HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
        if (hGameHandle)
        {
            SIZE_T uiBytesRead = 0;
            DWORD dwConnectedBaseAddress = 0;
            DWORD dwConnectedValue = 0;
            ReadProcessMemory(hGameHandle, (LPCVOID)(CH5::Offsets::dwHwDllBase + CH5::Offsets::dwIsConnected), (LPVOID)&dwConnectedBaseAddress, sizeof(dwConnectedBaseAddress), &uiBytesRead);
            if (uiBytesRead == sizeof(dwConnectedBaseAddress))
            {
                ReadProcessMemory(hGameHandle, (LPCVOID)(dwConnectedBaseAddress), (LPVOID)&dwConnectedValue, sizeof(dwConnectedValue), &uiBytesRead);
                if (uiBytesRead == sizeof(dwConnectedValue))
                    bConnected = (dwConnectedValue != 1);
            }
            CloseHandle(hGameHandle);
        }
    }
    */
    return (bFoundWindow);// && bConnected);
}