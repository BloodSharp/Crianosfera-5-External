#include "CH5_External.h"

playermove_t g_playermove;

void CH5::LocalPlayer::Update()
{
	if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwPlayerMoveS)
	{
		HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
		if (hGameHandle != INVALID_HANDLE_VALUE)
		{
			SIZE_T uiBytesRead = 0;
			ReadProcessMemory(hGameHandle, (LPCVOID)(CH5::Offsets::dwHwDllBase + CH5::Offsets::dwPlayerMoveS), (LPVOID)&g_playermove, sizeof(g_playermove), &uiBytesRead);
			CloseHandle(hGameHandle);
		}
	}
}

int CH5::LocalPlayer::GetIndex()
{
	return (g_playermove.player_index + 1);
}

int CH5::LocalPlayer::GetTeam()
{
	if ((g_playermove.player_index + 1) > 0 && (g_playermove.player_index + 1) < CH5_MAX_PLAYERS)
		return vPlayers[g_playermove.player_index + 1].GetTeam();
	return 0;
}

Vector CH5::LocalPlayer::GetEyePosition()
{
	return (g_playermove.origin + g_playermove.view_ofs);
}

int CH5::LocalPlayer::GetFlags()
{
	return g_playermove.flags;
}

bool CH5::LocalPlayer::IsAlive()
{
	//return vPlayers[CH5::LocalPlayer::GetIndex()].IsAlive();
	return vPlayers[CH5::LocalPlayer::GetIndex()].m_mdlModel.name[0] &&
		!(vPlayers[CH5::LocalPlayer::GetIndex()].m_clEntity.curstate.effects & EF_NODRAW) &&
		vPlayers[CH5::LocalPlayer::GetIndex()].m_clEntity.player &&
		!vPlayers[CH5::LocalPlayer::GetIndex()].m_clEntity.curstate.spectator &&
		vPlayers[CH5::LocalPlayer::GetIndex()].m_clEntity.curstate.solid;
}

Vector CH5::LocalPlayer::GetForwardVector()
{
	return g_playermove.forward;
}

int CH5::LocalPlayer::GetButtons()
{
	return g_playermove.cmd.buttons;
}

void CH5::LocalPlayer::SetViewAngles(Vector newangles)
{
	if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwViewAngles)
	{
		HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
		if (hGameHandle != INVALID_HANDLE_VALUE)
		{
			SIZE_T uiBytesRead = 0;
			WriteProcessMemory(hGameHandle, (LPVOID)(CH5::Offsets::dwHwDllBase + CH5::Offsets::dwViewAngles), (LPVOID)&newangles, sizeof(newangles), &uiBytesRead);
			CloseHandle(hGameHandle);
		}
	}
}