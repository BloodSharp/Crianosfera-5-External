// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "ValveSDK.h"

typedef struct cl_clientfunc_s
{
	int(*Initialize)(cl_enginefunc_t*, int);
	int(*HUD_Init)(void);
	int(*HUD_VidInit)(void);
	int(*HUD_Redraw)(float, int);
	int(*HUD_UpdateClientData)(client_data_t*, float);
	int(*HUD_Reset)(void);
	void(*HUD_PlayerMove)(struct playermove_s*, int);
	void(*HUD_PlayerMoveInit)(struct playermove_s*);
	char(*HUD_PlayerMoveTexture)(char*);
	void(*IN_ActivateMouse)(void);
	void(*IN_DeactivateMouse)(void);
	void(*IN_MouseEvent)(int mstate);
	void(*IN_ClearStates)(void);
	void(*IN_Accumulate)(void);
	void(*CL_CreateMove)(float, struct usercmd_s*, int);
	int(*CL_IsThirdPerson)(void);
	void(*CL_CameraOffset)(float*);
	struct kbutton_s* (*KB_Find)(const char* name);
	void(*CAM_Think)(void);
	void(*V_CalcRefdef)(struct ref_params_s* pparams);
	int(*HUD_AddEntity)(int, struct cl_entity_s*, const char*);
	void(*HUD_CreateEntities)(void);
	void(*HUD_DrawNormalTriangles)(void);
	void(*HUD_DrawTransparentTriangles)(void);
	void(*HUD_StudioEvent)(const struct mstudioevent_s*, const struct cl_entity_s*);
	void(*HUD_PostRunCmd)(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown)(void);
	void(*HUD_TxferLocalOverrides)(struct entity_state_s*, const struct clientdata_s*);
	void(*HUD_ProcessPlayerState)(struct entity_state_s*, const struct entity_state_s*);
	void(*HUD_TxferPredictionData)(struct entity_state_s*, const struct entity_state_s*, struct clientdata_s*, const struct clientdata_s*, struct weapon_data_s*, const struct weapon_data_s*);
	void(*Demo_ReadBuffer)(int, unsigned char*);
	int(*HUD_ConnectionlessPacket)(struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
	int(*HUD_GetHullBounds)(int hullnumber, float* mins, float* maxs);
	void(*HUD_Frame)(double time);
	int(*HUD_Key_Event)(int, int, const char*);
	void(*HUD_TempEntUpdate)(double, double, double, struct tempent_s**, struct tempent_s**, int(*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
	struct cl_entity_s* (*HUD_GetUserEntity)(int index);
	int(*HUD_VoiceStatus)(int entindex, qboolean bTalking);
	int (*HUD_DirectorEvent) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int (*HUD_GetStudioModelInterface)(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
} cl_clientfunc_t;


void DbgPrint(LPCSTR form, ...)
{
	static char tmp[2048];
	va_list p;
	va_start(p, form);
	vsprintf(tmp, form, p);
	MessageBox(0, tmp, "Information", MB_ICONINFORMATION);
	va_end(p);
}

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}

DWORD dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, char* szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}

void (*pOrig_HUD_PlayerMove)(struct playermove_s*, qboolean server) = 0;
void HOOK_HUD_PlayerMove(struct playermove_s*ppmove, qboolean server)
{
	static bool bOnce = false;
	pOrig_HUD_PlayerMove(ppmove, server);
	if (!bOnce)
	{
		bOnce = true;
		DbgPrint("[B#] ppmove: 0x%X", (PDWORD)ppmove);
	}
}

void ThreadProtocol48(void)
{
	HMODULE hHwDll = NULL;
	HMODULE hClientDll = NULL;

	cl_enginefunc_t* pEngfuncs = NULL;
	engine_studio_api_t* pStudio = NULL;
	cl_clientfunc_t* pClient = NULL;

	while (!hHwDll && !hClientDll)
	{
		hHwDll = GetModuleHandle("hw.dll");
		hClientDll = GetModuleHandle("client.dll");
		Sleep(17);
	}

	while (pClient == NULL || pEngfuncs == NULL || pStudio == NULL)
	{
		if (pClient == NULL)
		{
			//credits: Gordon
			DWORD dwClient = dwFindPattern((DWORD)hHwDll, 0x0122A000, (BYTE*)"\x8B\x44\x24\x04\x6A\x00\x68\x00\x00\x00\x00\x68", (char*)"xxxxxxx????x");
			if (dwClient)
			{
				pClient = (cl_clientfunc_t*)(*(PDWORD)(dwClient + 0x7));
				if (!pClient->Initialize)
					pClient = NULL;
				else
				{
					pOrig_HUD_PlayerMove = pClient->HUD_PlayerMove;
					pClient->HUD_PlayerMove = &HOOK_HUD_PlayerMove;
				}
			}
			else
				pClient = NULL;
		}

		if (pEngfuncs == NULL && hClientDll)
		{
			DWORD dwEngine = *(DWORD*)((DWORD)GetProcAddress(hClientDll, "Initialize") + 0x1C);
			if (dwEngine)
			{
				pEngfuncs = (cl_enginefunc_t*)(dwEngine);
				if (pEngfuncs->pfnSPR_Load && pEngfuncs->pTriAPI && pEngfuncs->pTriAPI->WorldToScreen)
				{
					double*globalTime = (double*)*(DWORD*)((DWORD)pEngfuncs->pNetAPI->SendRequest + 0x51);
					DbgPrint("[B#] globalTime: 0x%X", (PDWORD)globalTime);
					DbgPrint("[B#] WorldToScreen: hw.dll+0x%X", (PDWORD)pEngfuncs->pTriAPI->WorldToScreen - (PDWORD)hHwDll);
					DbgPrint("[B#] hw.dll: 0x%X", (PDWORD)hHwDll);
					DbgPrint("[B#] WorldToScreen: 0x%X", (PDWORD)pEngfuncs->pTriAPI->WorldToScreen);
					DbgPrint("[B#] GetEntityByIndex: 0x%X", (PDWORD)pEngfuncs->GetEntityByIndex);
					DbgPrint("[B#] cl_entity_t: 0x%X", sizeof(cl_entity_t));
					DbgPrint("[B#] Status: 0x%X", (PDWORD)pEngfuncs->pNetAPI->Status);
					DbgPrint("[B#] SetViewAngles: 0x%X", (PDWORD)pEngfuncs->SetViewAngles);
					DbgPrint("[B#] pfnGetCvarPointer: 0x%X", (PDWORD)pEngfuncs->pfnGetCvarPointer);
				}
				else
					pEngfuncs = NULL;
			}
			else
				pEngfuncs = NULL;
		}

		if (pStudio == NULL && hClientDll)
		{
			DWORD dwStudio = *(DWORD*)((DWORD)GetProcAddress(hClientDll, "HUD_GetStudioModelInterface") + 0x1A);
			if (dwStudio)
			{
				pStudio = (engine_studio_api_t*)(dwStudio);
				if (pStudio->Mem_Calloc)
				{
					DbgPrint("[B#] PlayerInfo: 0x%X", (PDWORD)pStudio->PlayerInfo);
				}
				else
					pStudio = NULL;
			}
			else
				pStudio = NULL;
		}
		Sleep(10);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadProtocol48, NULL, NULL, NULL);
    }
    return TRUE;
}

