#include "CH5_External.h"

bool CH5::Interface::EnableWindows = true;

void CH5::Interface::BackGround()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::SetNextWindowPos(ImVec2(0, 30));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("##CH5_BACKGROUND", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (CH5::Lua::Hooks::HasHook(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_RENDERING_BACKGROUND))
	{
		auto v = CH5::Lua::Hooks::GetCallbacks(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_RENDERING_BACKGROUND);
		for (unsigned int i = 0; i < v.size(); i++)
		{
			try
			{
				v[i]();
			}
			catch (luabridge::LuaException const& e)
			{
				if (CH5::Lua::pLuaState)
				{
					SetForegroundWindow(g_HackWnd);
					MessageBox(g_HackWnd, e.what(), 0, MB_ICONERROR);
					CH5::Lua::Hooks::RemoveAllCallbacks();
				}
			}
		}
	}

	
	if (CH5::Engine::IsGameActive())
	{
		for (int id = 1; id < CH5_MAX_PLAYERS; id++)
		{
			if (id == CH5::LocalPlayer::GetIndex())
				continue;
			if (vPlayers[id].IsAlive())
			{
				if (vPlayers[id].CanRenderAtScreen())
				{
					if (CH5::Settings::ESP::Enable)
					{
						vPlayers[id].RenderBox();
						vPlayers[id].RenderName();
						vPlayers[id].RenderModelName();
						vPlayers[id].RenderDistance();
						vPlayers[id].RenderSequence();
						vPlayers[id].RenderLatency();
					}
				}
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar(2);
}

void CH5::Interface::Windows(bool* bHackEnabled)
{
	if (CH5::Lua::Hooks::HasHook(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_RENDERING_INTERFACE))
	{
		auto v = CH5::Lua::Hooks::GetCallbacks(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_RENDERING_INTERFACE);
		for (unsigned int i = 0; i < v.size(); i++)
		{
			try
			{
				v[i]();
			}
			catch (luabridge::LuaException const& e)
			{
				if (CH5::Lua::pLuaState)
				{
					SetForegroundWindow(g_HackWnd);
					MessageBox(g_HackWnd, e.what(), 0, MB_ICONERROR);
					CH5::Lua::Hooks::RemoveAllCallbacks();
				}
			}
		}
	}
	if (CH5::Interface::EnableWindows)
	{
		if (ImGui::Begin("Crianosfera Hack v5 - External", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::BeginTabBar("##CH5_MAINTAB"))
			{
				if (ImGui::BeginTabItem("Credits"))
				{
					ImGui::Text("Press insert to activate / deactivate the interface");
					if (ImGui::Button("Close the hack"))
						*bHackEnabled = false;
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Aimbot##AIMBOT"))
				{
					ImGui::Checkbox("Enable##AIMBOT", &CH5::Settings::Aimbot::Enable);

					ImGui::BeginGroup();
					ImGui::Checkbox("Show spot##AIMBOT", &CH5::Settings::Aimbot::ShowSpot);
					ImGui::Checkbox("Prioritize short distance##AIMBOT", &CH5::Settings::Aimbot::PrioritizeIfSomeOneIsAtShortDistance);
					ImGui::EndGroup();

					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Combo("Aiming method##AIMBOT", &CH5::Settings::Aimbot::AimingMethod, "Field of View\0Distance\0\0");
					ImGui::Combo("Team##AIMBOT", &CH5::Settings::Aimbot::Team, "Enemies\0Teammates\0All\0\0");
					ImGui::EndGroup();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("ESP##ESP"))
				{
					ImGui::Checkbox("Enable##ESP", &CH5::Settings::ESP::Enable);
					ImGui::BeginGroup();
					ImGui::Checkbox("Name##ESP", &CH5::Settings::ESP::Name);
					ImGui::Checkbox("Box##ESP", &CH5::Settings::ESP::Box);
					ImGui::Checkbox("Distance##ESP", &CH5::Settings::ESP::Distance);
					ImGui::ColorEdit4("Counter-Terrorists##ESP", (float*)&CH5::Settings::ESP::Colors::CounterTerrorist, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					ImGui::EndGroup();

					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Checkbox("Model name##ESP", &CH5::Settings::ESP::ModelName);
					ImGui::Checkbox("Sequence##ESP", &CH5::Settings::ESP::Sequence);
					ImGui::Checkbox("Latency##ESP", &CH5::Settings::ESP::Latency);
					ImGui::ColorEdit4("Terrorists##ESP", (float*)&CH5::Settings::ESP::Colors::Terrorist, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					ImGui::EndGroup();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Misc"))
				{
					ImGui::Checkbox("ThirdPerson##MISC", &CH5::Settings::Misc::ThirdPerson);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Lua Scripts"))
				{
					if (ImGui::Button("Reload All Scripts"))
					{
						CH5::Lua::ReloadScripts();
					}
					if (CH5::Lua::Hooks::HasHook(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_RENDERING_MENU))
					{
						ImGui::Separator();
						auto v = CH5::Lua::Hooks::GetCallbacks(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_RENDERING_MENU);
						for (unsigned int i = 0; i < v.size(); i++)
						{
							try
							{
								v[i]();
							}
							catch (luabridge::LuaException const& e)
							{
								if (CH5::Lua::pLuaState)
								{
									SetForegroundWindow(g_HackWnd);
									MessageBox(g_HackWnd, e.what(), 0, MB_ICONERROR);
									CH5::Lua::Hooks::RemoveAllCallbacks();
								}
							}
						}
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}