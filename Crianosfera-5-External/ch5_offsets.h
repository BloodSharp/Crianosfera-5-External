#pragma once

#include "CH5_External.h"

namespace CH5
{
	namespace Offsets
	{
		extern DWORD dwProcessId;
		extern DWORD dwHwDllBase;
		extern DWORD dwClientDllBase;

		extern DWORD dwWorldToScreen;
		extern DWORD dwClEntityT;
		extern DWORD dwPlayerInfoS;
		extern DWORD dwIsConnected;

		extern DWORD dwPlayerTeam;
		extern DWORD dwPlayerMoveS;
		extern DWORD dwGlobalTime;
		extern DWORD dwViewAngles;

		extern DWORD dwGetFirstCvar;

		void InitializeOffsets(int nBuildVersion);
		void Clear();
	}
}