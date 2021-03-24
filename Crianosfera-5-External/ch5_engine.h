#pragma once

namespace CH5
{
	namespace Engine
	{
		extern int Width;
		extern int Height;
		bool WorldToScreen(Vector world, float* screen);
		bool GetCvarFloat(const char* szCvarName, float* pfValue);
		bool SetCvarFloat(const char* szCvarName, float fValue);
		void RePositionWindow();
		void ReadViewMatrix();
		bool IsGameActive();
	}
}