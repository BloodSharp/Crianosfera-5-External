#include "CH5_External.h"

DWORD CH5::Offsets::dwProcessId = 0;
DWORD CH5::Offsets::dwHwDllBase = 0;
DWORD CH5::Offsets::dwClientDllBase = 0;

DWORD CH5::Offsets::dwWorldToScreen = 0;
DWORD CH5::Offsets::dwClEntityT = 0;
DWORD CH5::Offsets::dwPlayerInfoS = 0;
DWORD CH5::Offsets::dwIsConnected = 0;

DWORD CH5::Offsets::dwPlayerTeam = 0;

DWORD CH5::Offsets::dwPlayerMoveS = 0;
DWORD CH5::Offsets::dwGlobalTime = 0;
DWORD CH5::Offsets::dwViewAngles = 0;

DWORD CH5::Offsets::dwGetFirstCvar = 0;

void CH5::Offsets::InitializeOffsets(int nBuildVersion)
{
	switch (nBuildVersion)
	{
		case 3266:
			break;
		case 4554:
			if (GetWindowThreadProcessId(FindWindow("Valve001", NULL), &dwProcessId))
			{
				dwHwDllBase = CH5::Utils::GetModuleAddress(dwProcessId, "hw.dll");
				dwClientDllBase = CH5::Utils::GetModuleAddress(dwProcessId, "client.dll");
				/*
				* [2604][B#] WorldToScreen: hw.dll + 0x27EA0
				* [1828][B#] WorldToScreen : 0x49CFA80 (0x04930000 + 0x9FA80)
				* sub_932A0-- > 0x049C32A9 + 2 = 0x49C32AB(0x57c56a0) - base = 0x932AB
				* 0x57c56a0 - base = 0xE956A0
				*/
				dwWorldToScreen = 0xE956A0;
				/*
				int __cdecl sub_19590(int a1)
				{
				  int result; // eax

				  off_17C8B4(&a1);
				  if ( a1 < 0 || a1 >= dword_102BDC0 )
					result = 0;
				  else
					result = dword_1AFBFC + 3000 * a1;
				  return result;
				}
				*/
				dwClEntityT = 0x1AFBFC;
				/*
				player_info_t *__cdecl studioapi_PlayerInfo(int index)
				{
				  player_info_t *result; // eax

				  result = 0;
				  if ( index >= 0 && index < ei.maxclients )
					result = &ei.players[index];
				  return result;
				}

				hw+0x9E3F0

				char *__cdecl sub_9E3F0(int a1)
				{
				  char *result; // eax

				  if ( a1 < 0 || a1 >= dword_11CF2A4 )
					result = 0;
				  else
					result = (char *)&unk_11CFEA8 + 592 * a1;
				  return result;
				}
				*/
				dwPlayerInfoS = 0x11CFEA8;
				/*
				void __cdecl sub_6F430(net_status_s *status)
				{
				  bool v1; // zf
				  int v2; // edx
				  int v3; // eax
				  int v4; // eax
				  int v5; // ecx
				  double v6; // ST10_8
				  int v7; // ecx
				  int v8; // eax
				  double v9; // st7
				  int v10; // [esp+10h] [ebp-8h]

				  if ( status )
				  {
					v1 = dword_10255C0 == 1;
					qmemcpy(&status->local_address, &dword_A6E640, sizeof(status->local_address));
					v2 = !v1;
					v10 = v2;
					status->connected = v2;
					qmemcpy(&status->remote_address, &dword_10255CC, sizeof(status->remote_address));
					if ( v1 )
					{
					  v3 = 0;
					}
					else
					{
					  v3 = sub_6F400();
					  v2 = v10;
					}
					status->packet_loss = v3;
					if ( v2 )
					{
					  v4 = dword_1AF6B0;
					  v5 = dword_1AF6B4;
					}
					else
					{
					  v4 = 0;
					  v5 = 0;
					}
					LODWORD(status->latency) = v4;
					HIDWORD(status->latency) = v5;
					if ( v2 )
					{
					  v6 = dbl_A83500 - flt_10255E8;
					  v7 = HIDWORD(v6);
					  v8 = LODWORD(v6);
					}
					else
					{
					  v8 = 0;
					  v7 = 0;
					}
					v9 = flt_16979C;
					LODWORD(status->connection_time) = v8;
					HIDWORD(status->connection_time) = v7;
					status->rate = v9;
				  }
				}
				*/
				dwIsConnected = 0x10255C0;

				/*
				int __cdecl HUD_GetPlayerTeam(signed int a1)
				{
				  int result; // eax

				  if ( a1 < 0 || a1 > 64 )
					result = 0;
				  else
					result = word_12B2F4[52 * a1];
				  return result;
				}
				*/
				dwPlayerTeam = 0x12B2F4;

				//ppmove: 0x5905AE0 / hw.dll: 0x4930000
				dwPlayerMoveS = 0xFD5AE0;

				//globaltime: 0x53B3500 / hw.dll: 0x4930000
				dwGlobalTime = 0xA83500;
				//SetViewAngles: 0x4949430 / hw.dll: 0x4930000 (0x19430)
				/*void __cdecl sub_19430(int *a1)
				{
				  int v1; // edx
				  int v2; // eax

				  off_17C86C(&a1);
				  v1 = a1[1];
				  v2 = a1[2];
				  dword_10569A4 = *a1;
				  dword_10569A8 = v1;
				  dword_10569AC = v2;
				}*/
				dwViewAngles = 0x10569A4;

				//pfnGetCvarPointer: 0x496B340 / hw.dll: 0x4930000 (0x3B340)
				/*cvar_t *__cdecl sub_3B340(const char *a1)
				{
				  cvar_t *v1; // esi

				  off_17C900(&a1);
				  v1 = (cvar_t *)dword_3367EC;
				  if ( !dword_3367EC )
					return 0;
				  while ( sub_368B0(a1, v1->name) )
				  {
					v1 = v1->next;
					if ( !v1 )
					  return 0;
				  }
				  return v1;
				}*/
				dwGetFirstCvar = 0x3367EC;
			}
			break;
	}
}

void CH5::Offsets::Clear()
{
	dwProcessId = 0;
	dwHwDllBase = 0;
	dwClientDllBase = 0;

	dwWorldToScreen = 0;
	dwClEntityT = 0;
	dwPlayerInfoS = 0;
	dwIsConnected = 0;

	dwPlayerTeam = 0;

	dwPlayerMoveS = 0;
	dwGlobalTime = 0;
	dwViewAngles = 0;

	dwGetFirstCvar = 0;
}