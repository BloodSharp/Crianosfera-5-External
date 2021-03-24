#pragma once

extern playermove_t g_playermove;

namespace CH5
{
	namespace LocalPlayer
	{
		void Update();
		int GetIndex();
		int GetTeam();
		Vector GetEyePosition();
		int GetFlags();
		bool IsAlive();
		Vector GetForwardVector();
		int GetButtons();
		void SetViewAngles(Vector newangles);
	}
}