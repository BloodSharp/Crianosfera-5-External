#include "CH5_External.h"

void CH5::Hooks::Update()
{
    //while (true)
    {
        // Toggle Windows
        if (GetAsyncKeyState(VK_INSERT) & 0x8000)
        {
            CH5::Interface::EnableWindows = !CH5::Interface::EnableWindows;
            if (!CH5::Interface::EnableWindows)
            {
                HWND hEngine = NULL;
                SetWindowLong(g_HackWnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT);
                if (hEngine = FindWindow("Valve001", NULL))
                    SetForegroundWindow(hEngine);
                CH5::Settings::Save();
            }
            else
            {
                SetForegroundWindow(g_HackWnd);
                SetWindowLong(g_HackWnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT);
            }
            Sleep(100);
        }

        if (CH5::Engine::IsGameActive())
        {
            if (!CH5::Offsets::dwProcessId)
                CH5::Offsets::InitializeOffsets(4554);
            CH5::Engine::RePositionWindow();
            CH5::Engine::ReadViewMatrix();
            CH5::LocalPlayer::Update();
            for (int id = 1; id < CH5_MAX_PLAYERS; id++)
                vPlayers[id].UpdateByIndex(id);

            if (CH5::Settings::Misc::ThirdPerson && CH5::LocalPlayer::IsAlive())
            {
                CH5::Engine::SetCvarFloat("chase_active", 1.0f);
                CH5::Engine::SetCvarFloat("r_drawviewmodel", 0.0f);
            }
            else
            {
                CH5::Engine::SetCvarFloat("chase_active", 0.0f);
                CH5::Engine::SetCvarFloat("r_drawviewmodel", 1.0f);
            }

            //CH5::Engine::SetCvarFloat("gl_wireframe", 2.0f);

            CH5::Aimbot::Run();

            if (CH5::Lua::Hooks::HasHook(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_UPDATE))
            {
                auto v = CH5::Lua::Hooks::GetCallbacks(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_UPDATE);
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
        }
        else
        {
            CH5::Offsets::Clear();
        }
        // Sleep to prevent CPU overload...
        //Sleep(1);
    }
}