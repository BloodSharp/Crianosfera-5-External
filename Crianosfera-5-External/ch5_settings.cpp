#include "CH5_External.h"

bool CH5::Settings::Aimbot::Enable = true;
int CH5::Settings::Aimbot::Team = 0;
bool CH5::Settings::Aimbot::ShowSpot = true;
bool CH5::Settings::Aimbot::PrioritizeIfSomeOneIsAtShortDistance = true;
int CH5::Settings::Aimbot::AimingMethod = 0;

bool CH5::Settings::ESP::Enable = true;
bool CH5::Settings::ESP::Box = true;
bool CH5::Settings::ESP::Name = true;
bool CH5::Settings::ESP::Distance = true;
bool CH5::Settings::ESP::ModelName = true;
bool CH5::Settings::ESP::Sequence = true;
bool CH5::Settings::ESP::Latency = true;

ImColor CH5::Settings::ESP::Colors::CounterTerrorist = ImColor(0, 128, 255);
ImColor CH5::Settings::ESP::Colors::Terrorist = ImColor(255, 128, 0);
ImColor CH5::Settings::ESP::Colors::Aimbot = ImColor(0, 255, 0);

bool CH5::Settings::Misc::ThirdPerson = false;

void GetVal(Json::Value& config, int* setting)
{
    if (config.isNull())
        return;

    *setting = config.asInt();
}

void GetVal(Json::Value& config, bool* setting)
{
    if (config.isNull())
        return;

    *setting = config.asBool();
}

void GetVal(Json::Value& config, float* setting)
{
    if (config.isNull())
        return;

    *setting = config.asFloat();
}

void GetVal(Json::Value& config, ImColor* setting)
{
    if (config.isNull())
        return;

    int r, g, b, a;

    GetVal(config["Red"], &r);
    GetVal(config["Green"], &g);
    GetVal(config["Blue"], &b);
    GetVal(config["Alpha"], &a);
    setting->Value.x = float(r) / 255;
    setting->Value.y = float(g) / 255;
    setting->Value.z = float(b) / 255;
    setting->Value.w = float(a) / 255;
}

void SaveColor(Json::Value& config, ImColor color)
{
    config["Red"] = int(color.Value.x * 255);
    config["Green"] = int(color.Value.y * 255);
    config["Blue"] = int(color.Value.z * 255);
    config["Alpha"] = int(color.Value.w * 255);
}

void CH5::Settings::Initialize()
{

}

void CH5::Settings::Save()
{
    Json::Value settings;
    Json::StyledWriter styledWriter;

    settings["Aimbot"]["Enable"] = CH5::Settings::Aimbot::Enable;
    settings["Aimbot"]["Team"] = CH5::Settings::Aimbot::Team;
    settings["Aimbot"]["ShowSpot"] = CH5::Settings::Aimbot::ShowSpot;
    settings["Aimbot"]["PrioritizeIfSomeOneIsAtShortDistance"] = CH5::Settings::Aimbot::PrioritizeIfSomeOneIsAtShortDistance;
    settings["Aimbot"]["AimingMethod"] = CH5::Settings::Aimbot::AimingMethod;

    settings["ESP"]["Enable"] = CH5::Settings::ESP::Enable;
    settings["ESP"]["Box"] = CH5::Settings::ESP::Box;
    settings["ESP"]["Name"] = CH5::Settings::ESP::Name;
    settings["ESP"]["Distance"] = CH5::Settings::ESP::Distance;
    settings["ESP"]["ModelName"] = CH5::Settings::ESP::ModelName;
    settings["ESP"]["Sequence"] = CH5::Settings::ESP::Sequence;
    settings["ESP"]["Latency"] = CH5::Settings::ESP::Latency;

    SaveColor(settings["ESP"]["Colors"]["CounterTerrorist"], CH5::Settings::ESP::Colors::CounterTerrorist);
    SaveColor(settings["ESP"]["Colors"]["Terrorist"], CH5::Settings::ESP::Colors::Terrorist);
    SaveColor(settings["ESP"]["Colors"]["Aimbot"], CH5::Settings::ESP::Colors::Aimbot);

    std::ofstream file;
    file.open("Crianosfera.json");
    if (file.is_open())
    {
        file << styledWriter.write(settings);
        file.close();
    }

    if (CH5::Lua::Hooks::HasHook(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_SAVE_SETTINGS))
    {
        auto v = CH5::Lua::Hooks::GetCallbacks(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_SAVE_SETTINGS);
        for (unsigned int i = 0; i < v.size(); i++)
        {
            try
            {
                v[i](std::string("Crianosfera.json"));
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

void CH5::Settings::Load()
{
    if (!std::ifstream("Crianosfera.json").good())
    {
        CH5::Settings::Save();
        return;
    }

    Json::Value settings;
    std::ifstream configDoc("Crianosfera.json", std::ifstream::binary);
    configDoc >> settings;

    GetVal(settings["Aimbot"]["Enable"], &CH5::Settings::Aimbot::Enable);
    GetVal(settings["Aimbot"]["Team"], &CH5::Settings::Aimbot::Team);
    GetVal(settings["Aimbot"]["ShowSpot"], &CH5::Settings::Aimbot::ShowSpot);
    GetVal(settings["Aimbot"]["PrioritizeIfSomeOneIsAtShortDistance"], &CH5::Settings::Aimbot::PrioritizeIfSomeOneIsAtShortDistance);
    GetVal(settings["Aimbot"]["AimingMethod"], &CH5::Settings::Aimbot::AimingMethod);

    GetVal(settings["ESP"]["Enable"], &CH5::Settings::ESP::Enable);
    GetVal(settings["ESP"]["Box"], &CH5::Settings::ESP::Box);
    GetVal(settings["ESP"]["Name"], &CH5::Settings::ESP::Name);
    GetVal(settings["ESP"]["Distance"], &CH5::Settings::ESP::Distance);
    GetVal(settings["ESP"]["ModelName"], &CH5::Settings::ESP::ModelName);
    GetVal(settings["ESP"]["Sequence"], &CH5::Settings::ESP::Sequence);
    GetVal(settings["ESP"]["Latency"], &CH5::Settings::ESP::Latency);

    GetVal(settings["ESP"]["Colors"]["CounterTerrorist"], &CH5::Settings::ESP::Colors::CounterTerrorist);
    GetVal(settings["ESP"]["Colors"]["Terrorist"], &CH5::Settings::ESP::Colors::Terrorist);
    GetVal(settings["ESP"]["Colors"]["Aimbot"], &CH5::Settings::ESP::Colors::Aimbot);

    if (CH5::Lua::Hooks::HasHook(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_LOAD_SETTINGS))
    {
        auto v = CH5::Lua::Hooks::GetCallbacks(CH5::Lua::CH5_CALLBACK_TYPE::CH5_CALLBACK_AT_LOAD_SETTINGS);
        for (unsigned int i = 0; i < v.size(); i++)
        {
            try
            {
                v[i](std::string("Crianosfera.json"));
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