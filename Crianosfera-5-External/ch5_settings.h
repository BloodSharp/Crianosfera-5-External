#pragma once

void GetVal(Json::Value& config, int* setting);
void GetVal(Json::Value& config, bool* setting);
void GetVal(Json::Value& config, float* setting);
void GetVal(Json::Value& config, ImColor* setting);
void SaveColor(Json::Value& config, ImColor color);

namespace CH5
{
	namespace Settings
	{
		namespace Aimbot
		{
			extern bool Enable;
			extern int Team;
			extern bool ShowSpot;
			extern bool PrioritizeIfSomeOneIsAtShortDistance;
			extern int AimingMethod;
		}

		namespace ESP
		{
			extern bool Enable;
			extern bool Box;
			extern bool Name;
			extern bool Distance;
			extern bool ModelName;
			extern bool Sequence;
			extern bool Latency;

			namespace Colors
			{
				extern ImColor CounterTerrorist;
				extern ImColor Terrorist;
				extern ImColor Aimbot;
			}
		}

		namespace Misc
		{
			extern bool ThirdPerson;
		}
		void Initialize();
		void Save();
		void Load();
	}
}