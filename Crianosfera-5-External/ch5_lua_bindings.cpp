#include "CH5_External.h"

lua_State* CH5::Lua::pLuaState = NULL;

std::unordered_map<UINT, std::vector<luabridge::LuaRef>> CH5::Lua::Hooks::Callbacks;
Vector CH5::Lua::Utils::vLastConvertedVector(0, 0, 0);

void CH5::Lua::Hooks::RegisterCallBack(UINT type, luabridge::LuaRef f)
{
	if (type < 0 || type >= CH5_CALLBACK_ALL_CALLBACKS)
		return;
	CH5::Lua::Hooks::Callbacks[type].push_back(f);
}

std::vector<luabridge::LuaRef> CH5::Lua::Hooks::GetCallbacks(CH5_CALLBACK_TYPE type)
{
	return CH5::Lua::Hooks::Callbacks[type];
}

bool CH5::Lua::Hooks::HasHook(CH5_CALLBACK_TYPE type)
{
	auto it = CH5::Lua::Hooks::Callbacks.find(type);
	return it != CH5::Lua::Hooks::Callbacks.end();
}

void CH5::Lua::Hooks::RemoveAllCallbacks()
{
	for (unsigned int i = 0; i < CH5_CALLBACK_ALL_CALLBACKS; i++)
		CH5::Lua::Hooks::Callbacks[i].clear();
}

bool CH5::Lua::Utils::ConvertToScreen(Vector& in)
{
	return CH5::Engine::WorldToScreen((float*)in, (float*)CH5::Lua::Utils::vLastConvertedVector);
}

Vector CH5::Lua::Utils::GetLastConvertedToScreenVector()
{
	return CH5::Lua::Utils::vLastConvertedVector;
}

int CH5::Lua::Utils::LoadInteger(const char* szFile, const char* szValue)
{
	int iTheValue = 0;
	if (!std::ifstream(szFile).good())
		return iTheValue;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szValue], &iTheValue);
		configDoc.close();
	}
	return iTheValue;
}

float CH5::Lua::Utils::LoadFloat(const char* szFile, const char* szValue)
{
	float fTheValue = 0;
	if (!std::ifstream(szFile).good())
		return fTheValue;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary); 
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szValue], &fTheValue);
		configDoc.close();
	}
	return fTheValue;
}

bool CH5::Lua::Utils::LoadBoolean(const char* szFile, const char* szValue)
{
	bool bTheValue = false;
	if (!std::ifstream(szFile).good())
		return bTheValue;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szValue], &bTheValue);
		configDoc.close();
	}
	return bTheValue;
}

ImColor CH5::Lua::Utils::LoadColor(const char* szFile, const char* szValue)
{
	ImColor TheColor = ImColor(0, 0, 0, 0);
	if (!std::ifstream(szFile).good())
		return TheColor;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szValue], (ImColor*)&TheColor);
		configDoc.close();
	}
	return TheColor;
}

void CH5::Lua::Utils::SaveInteger(const char* szFile, const char* szValue, int iValue)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szValue] = iValue;
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

void CH5::Lua::Utils::SaveFloat(const char* szFile, const char* szValue, float fValue)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szValue] = fValue;
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

void CH5::Lua::Utils::SaveBoolean(const char* szFile, const char* szValue, bool bValue)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szValue] = bValue;
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

void CH5::Lua::Utils::SaveColor(const char* szFile, const char* szValue, ImColor& color)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szValue]["Red"] = int(color.Value.x * 255);
	settings["Scripts"][szValue]["Green"] = int(color.Value.y * 255);
	settings["Scripts"][szValue]["Blue"] = int(color.Value.z * 255);
	settings["Scripts"][szValue]["Alpha"] = int(color.Value.w * 255);
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

int CH5::Lua::Utils::LoadIntegerInSection(const char* szFile, const char* szSection, const char* szValue)
{
	int iTheValue = 0;
	if (!std::ifstream(szFile).good())
		return iTheValue;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szSection][szValue], &iTheValue);
		configDoc.close();
	}
	return iTheValue;
}

float CH5::Lua::Utils::LoadFloatInSection(const char* szFile, const char* szSection, const char* szValue)
{
	float fTheValue = 0;
	if (!std::ifstream(szFile).good())
		return fTheValue;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szSection][szValue], &fTheValue);
		configDoc.close();
	}
	return fTheValue;
}

bool CH5::Lua::Utils::LoadBooleanInSection(const char* szFile, const char* szSection, const char* szValue)
{
	bool bTheValue = false;
	if (!std::ifstream(szFile).good())
		return bTheValue;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szSection][szValue], &bTheValue);
		configDoc.close();
	}
	return bTheValue;
}

ImColor CH5::Lua::Utils::LoadColorInSection(const char* szFile, const char* szSection, const char* szValue)
{
	ImColor TheColor = ImColor(0, 0, 0, 0);
	if (!std::ifstream(szFile).good())
		return TheColor;
	Json::Value settings;
	std::ifstream configDoc(szFile, std::ifstream::binary);
	if (configDoc.is_open())
	{
		configDoc >> settings;
		GetVal(settings["Scripts"][szSection][szValue], (ImColor*)&TheColor);
		configDoc.close();
	}
	return TheColor;
}

void CH5::Lua::Utils::SaveIntegerInSection(const char* szFile, const char* szSection, const char* szValue, int iValue)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szSection][szValue] = iValue;
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

void CH5::Lua::Utils::SaveFloatInSection(const char* szFile, const char* szSection, const char* szValue, float fValue)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szSection][szValue] = fValue;
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

void CH5::Lua::Utils::SaveBooleanInSection(const char* szFile, const char* szSection, const char* szValue, bool bValue)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szSection][szValue] = bValue;
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

void CH5::Lua::Utils::SaveColorInSection(const char* szFile, const char* szSection, const char* szValue, ImColor& color)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;
	if (std::ifstream(szFile).good())
	{
		std::ifstream configDoc(szFile, std::ifstream::binary);
		if (configDoc.is_open())
		{
			configDoc >> settings;
			configDoc.close();
		}
	}
	settings["Scripts"][szSection][szValue]["Red"] = int(color.Value.x * 255);
	settings["Scripts"][szSection][szValue]["Green"] = int(color.Value.y * 255);
	settings["Scripts"][szSection][szValue]["Blue"] = int(color.Value.z * 255);
	settings["Scripts"][szSection][szValue]["Alpha"] = int(color.Value.w * 255);
	std::ofstream file;
	file.open(szFile);
	if (file.is_open())
	{
		file << styledWriter.write(settings);
		file.close();
	}
}

int CH5::Lua::LocalPlayer::GetIndex()
{
	return ::CH5::LocalPlayer::GetIndex();
}

int CH5::Lua::LocalPlayer::GetTeam()
{
	return ::CH5::LocalPlayer::GetTeam();
}

Vector CH5::Lua::LocalPlayer::GetEyePosition()
{
	return ::CH5::LocalPlayer::GetEyePosition();
}

int CH5::Lua::LocalPlayer::GetFlags()
{
	return ::CH5::LocalPlayer::GetFlags();
}

bool CH5::Lua::LocalPlayer::IsAlive()
{
	return ::CH5::LocalPlayer::IsAlive();
}

Vector CH5::Lua::LocalPlayer::GetForwardVector()
{
	return ::CH5::LocalPlayer::GetForwardVector();
}

int CH5::Lua::LocalPlayer::GetButtons()
{
	return ::CH5::LocalPlayer::GetButtons();
}

void CH5::Lua::LocalPlayer::SetViewAngles(Vector& newangles)
{
	::CH5::LocalPlayer::SetViewAngles(newangles);
}

int CH5::Lua::EntityManager::GetTeam(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	return vPlayers[index].GetTeam();
}

Vector CH5::Lua::EntityManager::GetOrigin(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return Vector(0, 0, 0);
	return vPlayers[index].m_vOrigin;
}

std::string CH5::Lua::EntityManager::GetName(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	return std::string(vPlayers[index].m_stPlayerInfo.name);
}

std::string CH5::Lua::EntityManager::GetModelName(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	return std::string(vPlayers[index].m_mdlModel.name);
}

int CH5::Lua::EntityManager::GetDistance(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	Vector vDifference = vPlayers[index].m_vOrigin - CH5::LocalPlayer::GetEyePosition();
	int iDistance = int(vDifference.Length() / 22.0f);
	return iDistance;
}

float CH5::Lua::EntityManager::GetActualDistance(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	Vector vDifference = vPlayers[index].m_vOrigin - CH5::LocalPlayer::GetEyePosition();
	return vDifference.Length();
}

int CH5::Lua::EntityManager::GetPing(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	return vPlayers[index].m_stPlayerInfo.ping;
}

bool CH5::Lua::EntityManager::IsAlive(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return false;
	return vPlayers[index].IsAlive();
}

int CH5::Lua::EntityManager::GetSequence(int index)
{
	if (index < 1 || index >= CH5_MAX_PLAYERS)
		return 0;
	return Cstrike_SequenceInfo[vPlayers[index].m_clEntity.curstate.sequence];
}

bool CH5::Lua::ImGui::Begin(const char* szTitle, ImGuiWindowFlags flags)
{
	return ::ImGui::Begin(szTitle, 0, flags);
}

void CH5::Lua::ImGui::End()
{
	::ImGui::End();
}

void CH5::Lua::ImGui::Text(const char* szText)
{
	::ImGui::Text(szText);
}

bool CH5::Lua::ImGui::Checkbox(const char* szText, bool bCurrentValue)
{
	bool bTheValue = bCurrentValue;
	::ImGui::Checkbox(szText, &bTheValue);
	return bTheValue;
}

void CH5::Lua::ImGui::SameLine()
{
	::ImGui::SameLine();
}

void CH5::Lua::ImGui::NewLine()
{
	::ImGui::NewLine();
}

bool CH5::Lua::ImGui::TreeNode(const char* szTitle, bool bDefaultOpen)
{
	return ::ImGui::TreeNodeEx(szTitle, bDefaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0);
}

void CH5::Lua::ImGui::TreePop()
{
	::ImGui::TreePop();
}

bool CH5::Lua::ImGui::SmallButton(const char* szText)
{
	return ::ImGui::SmallButton(szText);
}

int CH5::Lua::ImGui::Combo(const char* szText, int iCurrentValue, const char* szOptions)
{
	int iTheValue = iCurrentValue;
	::ImGui::Combo(szText, &iTheValue, szOptions);
	return iTheValue;
}

void CH5::Lua::ImGui::BeginGroup()
{
	::ImGui::BeginGroup();
}

void CH5::Lua::ImGui::EndGroup()
{
	::ImGui::EndGroup();
}

bool CH5::Lua::ImGui::BeginTabBar(const char* szId)
{
	return ::ImGui::BeginTabBar(szId);
}

void CH5::Lua::ImGui::EndTabBar()
{
	::ImGui::EndTabBar();
}

bool CH5::Lua::ImGui::BeginTabItem(const char* szTitle)
{
	return ::ImGui::BeginTabItem(szTitle);
}

void CH5::Lua::ImGui::EndTabItem()
{
	::ImGui::EndTabItem();
}

int CH5::Lua::ImGui::GetFramesPerSecond()
{
	return (int)::ImGui::GetIO().Framerate;
}

void CH5::Lua::ImGui::PushItemWidth(int width)
{
	::ImGui::PushItemWidth((float)width);
}

void CH5::Lua::ImGui::PopItemWidth()
{
	::ImGui::PopItemWidth();
}

bool CH5::Lua::ImGui::IsItemClicked(ImGuiMouseButton mouse_button)
{
	return ::ImGui::IsItemClicked(mouse_button);
}

ImColor CH5::Lua::ImGui::ColorEdit4(const char* szColor, ImColor& color, int flags)
{
	::ImGui::ColorEdit4(szColor, (float*)&color, flags);
	return color;
}

void CH5::Lua::ImGui::Render::AddRect(ImVec2& v1, ImVec2& v2, ImColor& color, float rounding, int corners, float thickness)
{
	::ImGui::GetWindowDrawList()->AddRect(v1, v2, color, rounding, corners, thickness);
}

void CH5::Lua::ImGui::Render::AddLine(ImVec2& v1, ImVec2& v2, ImColor& color, float thickness)
{
	::ImGui::GetWindowDrawList()->AddLine(v1, v2, color, thickness);
}

void CH5::Lua::ImGui::Render::AddText(ImVec2& position, ImColor& color, const char* szText)
{
	::ImGui::GetWindowDrawList()->AddText(position, color, szText);
}

void CH5::Lua::ImGui::Render::AddRectFilled(ImVec2& v1, ImVec2& v2, ImColor& color, float rounding, int corners)
{
	::ImGui::GetWindowDrawList()->AddRectFilled(v1, v2, color, rounding, corners);
}

void DefineLuaGlobal(lua_State* L, const char* name, int value)
{
	lua_pushinteger(L, value);
	lua_setglobal(L, name);
}

bool CH5::Lua::Initialize()
{
	CH5::Lua::pLuaState = luaL_newstate();
	if (!CH5::Lua::pLuaState)
	{
		MessageBox(g_HackWnd, "Couldn't initialize Lua State", "Crianosfera Hack v5 - BloodSharp", MB_ICONERROR);
		return false;
	}
	luaL_openlibs(CH5::Lua::pLuaState);

	luabridge::getGlobalNamespace(CH5::Lua::pLuaState)

		.beginClass<ImVec2>("ImVec2")
			.addConstructor<void(*)(float, float)>()
			.addProperty("x", &ImVec2::x)
			.addProperty("y", &ImVec2::y)
		.endClass()

		.beginClass<ImVec4>("ImVec4")
			.addConstructor<void(*)(float, float, float, float)>()
			.addProperty("x", &ImVec4::x)
			.addProperty("y", &ImVec4::y)
			.addProperty("z", &ImVec4::z)
			.addProperty("w", &ImVec4::w)
		.endClass()

		.beginClass<ImColor>("ImColor")
			.addConstructor<void(*)(int, int, int, int)>()
			.addProperty("Value", &ImColor::Value)
		.endClass()

		.beginClass<Vector2D>("Vector2D")
			.addConstructor<void(*)(float, float)>()
			.addProperty("x", &Vector2D::x)
			.addProperty("y", &Vector2D::y)
			.addFunction("Length", &Vector2D::Length)
			.addFunction("Normalize", &Vector2D::Normalize)
			.addFunction("__add", &Vector2D::operator+)
			.addFunction("__sub", &Vector2D::operator-)
			.addFunction("__mul", &Vector2D::operator*)
			.addFunction("__div", &Vector2D::operator/)
		.endClass()

		.beginClass<Vector>("Vector")
			.addConstructor<void(*)(float, float, float)>()
			.addProperty("x", &Vector::x)
			.addProperty("y", &Vector::y)
			.addProperty("z", &Vector::z)
			.addFunction("Length", &Vector::Length)
			.addFunction("Length2D", &Vector::Length2D)
			.addFunction("Normalize", &Vector::Normalize)
			.addFunction("IsZero", &Vector::IsZero)
			.addFunction("IsZero2D", &Vector::IsZero2D)
			.addFunction("Make2D", &Vector::Make2D)
			.addFunction("__add", &Vector::operator+)
			.addFunction("__sub", &Vector::operator-)
			.addFunction("__mul", &Vector::operator*)
			.addFunction("__div", &Vector::operator/)
			.addFunction("__eq", &Vector::operator==)
		.endClass()

		.beginNamespace("Hooks")
			.addFunction("RegisterCallback", &CH5::Lua::Hooks::RegisterCallBack)
		.endNamespace()

		.beginNamespace("Utils")
			.addFunction("ConvertToScreen", &CH5::Lua::Utils::ConvertToScreen)
			.addFunction("GetLastConvertedToScreenVector", &CH5::Lua::Utils::GetLastConvertedToScreenVector)
			.addFunction("LoadInteger", &CH5::Lua::Utils::LoadInteger)
			.addFunction("LoadFloat", &CH5::Lua::Utils::LoadFloat)
			.addFunction("LoadBoolean", &CH5::Lua::Utils::LoadBoolean)
			.addFunction("LoadColor", &CH5::Lua::Utils::LoadColor)
			.addFunction("SaveInteger", &CH5::Lua::Utils::SaveInteger)
			.addFunction("SaveFloat", &CH5::Lua::Utils::SaveFloat)
			.addFunction("SaveBoolean", &CH5::Lua::Utils::SaveBoolean)
			.addFunction("SaveColor", &CH5::Lua::Utils::SaveColor)
			.addFunction("LoadIntegerInSection", &CH5::Lua::Utils::LoadIntegerInSection)
			.addFunction("LoadFloatInSection", &CH5::Lua::Utils::LoadFloatInSection)
			.addFunction("LoadBooleanInSection", &CH5::Lua::Utils::LoadBooleanInSection)
			.addFunction("LoadColorInSection", &CH5::Lua::Utils::LoadColorInSection)
			.addFunction("SaveIntegerInSection", &CH5::Lua::Utils::SaveIntegerInSection)
			.addFunction("SaveFloatInSection", &CH5::Lua::Utils::SaveFloatInSection)
			.addFunction("SaveBooleanInSection", &CH5::Lua::Utils::SaveBooleanInSection)
			.addFunction("SaveColorInSection", &CH5::Lua::Utils::SaveColorInSection)
		.endNamespace()

		.beginNamespace("LocalPlayer")
			.addFunction("GetIndex", &CH5::Lua::LocalPlayer::GetIndex)
			.addFunction("GetTeam", &CH5::Lua::LocalPlayer::GetTeam)
			.addFunction("GetEyePosition", &CH5::Lua::LocalPlayer::GetEyePosition)
			.addFunction("GetFlags", &CH5::Lua::LocalPlayer::GetFlags)
			.addFunction("IsAlive", &CH5::Lua::LocalPlayer::IsAlive)
			.addFunction("GetForwardVector", &CH5::Lua::LocalPlayer::GetForwardVector)
			.addFunction("GetButtons", &CH5::Lua::LocalPlayer::GetButtons)
			.addFunction("SetViewAngles", &CH5::Lua::LocalPlayer::SetViewAngles)
		.endNamespace()

		.beginNamespace("EntityManager")
			.addFunction("GetTeam", &CH5::Lua::EntityManager::GetTeam)
			.addFunction("GetOrigin", &CH5::Lua::EntityManager::GetOrigin)
			.addFunction("GetName", &CH5::Lua::EntityManager::GetName)
			.addFunction("GetModelName", &CH5::Lua::EntityManager::GetModelName)
			.addFunction("GetDistance", &CH5::Lua::EntityManager::GetDistance)
			.addFunction("GetActualDistance", &CH5::Lua::EntityManager::GetActualDistance)
			.addFunction("GetPing", &CH5::Lua::EntityManager::GetPing)
			.addFunction("IsAlive", &CH5::Lua::EntityManager::IsAlive)
			.addFunction("GetSequence", &CH5::Lua::EntityManager::GetSequence)
		.endNamespace()

		.beginNamespace("ImGui")
			.addFunction("Begin", &CH5::Lua::ImGui::Begin)
			.addFunction("End", &CH5::Lua::ImGui::End)
			.addFunction("Text", &CH5::Lua::ImGui::Text)
			.addFunction("Checkbox", &CH5::Lua::ImGui::Checkbox)
			.addFunction("SameLine", &CH5::Lua::ImGui::SameLine)
			.addFunction("NewLine", &CH5::Lua::ImGui::NewLine)
			.addFunction("TreeNode", &CH5::Lua::ImGui::TreeNode)
			.addFunction("TreePop", &CH5::Lua::ImGui::TreePop)
			.addFunction("SmallButton", &CH5::Lua::ImGui::SmallButton)
			.addFunction("Combo", &CH5::Lua::ImGui::Combo)
			.addFunction("BeginGroup", &CH5::Lua::ImGui::BeginGroup)
			.addFunction("EndGroup", &CH5::Lua::ImGui::EndGroup)
			.addFunction("BeginTabBar", &CH5::Lua::ImGui::BeginTabBar)
			.addFunction("EndTabBar", &CH5::Lua::ImGui::EndTabBar)
			.addFunction("BeginTabItem", &CH5::Lua::ImGui::BeginTabItem)
			.addFunction("EndTabItem", &CH5::Lua::ImGui::EndTabItem)
			.addFunction("GetFramesPerSecond", &CH5::Lua::ImGui::GetFramesPerSecond)
			.addFunction("PushItemWidth", &CH5::Lua::ImGui::PushItemWidth)
			.addFunction("PopItemWidth", &CH5::Lua::ImGui::PopItemWidth)
			.addFunction("IsItemClicked", &CH5::Lua::ImGui::IsItemClicked)
			.addFunction("ColorEdit4", &CH5::Lua::ImGui::ColorEdit4)

			.beginNamespace("Render")
				.addFunction("AddRect", &CH5::Lua::ImGui::Render::AddRect)
				.addFunction("AddLine", &CH5::Lua::ImGui::Render::AddLine)
				.addFunction("AddText", &CH5::Lua::ImGui::Render::AddText)
				.addFunction("AddRectFilled", &CH5::Lua::ImGui::Render::AddRectFilled)
			.endNamespace()
		.endNamespace()
		;
	DefineLuaGlobal(CH5::Lua::pLuaState, "CH5_CALLBACK_AT_RENDERING_INTERFACE", CH5_CALLBACK_AT_RENDERING_INTERFACE);
	DefineLuaGlobal(CH5::Lua::pLuaState, "CH5_CALLBACK_AT_RENDERING_BACKGROUND", CH5_CALLBACK_AT_RENDERING_BACKGROUND);
	DefineLuaGlobal(CH5::Lua::pLuaState, "CH5_CALLBACK_AT_RENDERING_MENU", CH5_CALLBACK_AT_RENDERING_MENU);
	DefineLuaGlobal(CH5::Lua::pLuaState, "CH5_CALLBACK_AT_UPDATE", CH5_CALLBACK_AT_UPDATE);
	DefineLuaGlobal(CH5::Lua::pLuaState, "CH5_CALLBACK_AT_LOAD_SETTINGS", CH5_CALLBACK_AT_LOAD_SETTINGS);
	DefineLuaGlobal(CH5::Lua::pLuaState, "CH5_CALLBACK_AT_SAVE_SETTINGS", CH5_CALLBACK_AT_SAVE_SETTINGS);

	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_IDLE", SEQUENCE_IDLE);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_SHOOT", SEQUENCE_SHOOT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_RELOAD", SEQUENCE_RELOAD);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_DIE", SEQUENCE_DIE);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_THROW", SEQUENCE_THROW);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_ARM_C4", SEQUENCE_ARM_C4);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_SHIELD", SEQUENCE_SHIELD);
	DefineLuaGlobal(CH5::Lua::pLuaState, "SEQUENCE_SHIELD_SIDE", SEQUENCE_SHIELD_SIDE);

	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_FLY", FL_FLY);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_SWIM", FL_SWIM);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_CONVEYOR", FL_CONVEYOR);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_CLIENT", FL_CLIENT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_INWATER", FL_INWATER);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_MONSTER", FL_MONSTER);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_GODMODE", FL_GODMODE);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_NOTARGET", FL_NOTARGET);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_SKIPLOCALHOST", FL_SKIPLOCALHOST);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_ONGROUND", FL_ONGROUND);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_PARTIALGROUND", FL_PARTIALGROUND);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_WATERJUMP", FL_WATERJUMP);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_FROZEN", FL_FROZEN);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_FAKECLIENT", FL_FAKECLIENT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_DUCKING", FL_DUCKING);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_FLOAT", FL_FLOAT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_GRAPHED", FL_GRAPHED);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_IMMUNE_WATER", FL_IMMUNE_WATER);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_IMMUNE_SLIME", FL_IMMUNE_SLIME);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_IMMUNE_LAVA", FL_IMMUNE_LAVA);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_PROXY", FL_PROXY);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_ALWAYSTHINK", FL_ALWAYSTHINK);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_BASEVELOCITY", FL_BASEVELOCITY);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_MONSTERCLIP", FL_MONSTERCLIP);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_ONTRAIN", FL_ONTRAIN);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_WORLDBRUSH", FL_WORLDBRUSH);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_SPECTATOR", FL_SPECTATOR);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_CUSTOMENTITY", FL_CUSTOMENTITY);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_KILLME", FL_KILLME);
	DefineLuaGlobal(CH5::Lua::pLuaState, "FL_DORMANT", FL_DORMANT);

	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_ATTACK", IN_ATTACK);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_JUMP", IN_JUMP);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_DUCK", IN_DUCK);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_FORWARD", IN_FORWARD);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_BACK", IN_BACK);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_USE", IN_USE);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_CANCEL", IN_CANCEL);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_LEFT", IN_LEFT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_RIGHT", IN_RIGHT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_MOVELEFT", IN_MOVELEFT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_MOVERIGHT", IN_MOVERIGHT);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_ATTACK2", IN_ATTACK2);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_RUN", IN_RUN);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_RELOAD", IN_RELOAD);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_ALT1", IN_ALT1);
	DefineLuaGlobal(CH5::Lua::pLuaState, "IN_SCORE", IN_SCORE);

	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_None", ImGuiWindowFlags_None);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoTitleBar", ImGuiWindowFlags_NoTitleBar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoResize", ImGuiWindowFlags_NoResize);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoMove", ImGuiWindowFlags_NoMove);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoScrollbar", ImGuiWindowFlags_NoScrollbar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoCollapse", ImGuiWindowFlags_NoCollapse);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoBackground", ImGuiWindowFlags_NoBackground);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoSavedSettings", ImGuiWindowFlags_NoSavedSettings);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoMouseInputs", ImGuiWindowFlags_NoMouseInputs);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_MenuBar", ImGuiWindowFlags_MenuBar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoNavInputs", ImGuiWindowFlags_NoNavInputs);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoNavFocus", ImGuiWindowFlags_NoNavFocus);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_UnsavedDocument", ImGuiWindowFlags_UnsavedDocument);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoNav", ImGuiWindowFlags_NoNav);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoDecoration", ImGuiWindowFlags_NoDecoration);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiWindowFlags_NoInputs", ImGuiWindowFlags_NoInputs);

	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiMouseButton_Left", ImGuiMouseButton_Left);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiMouseButton_Right", ImGuiMouseButton_Right);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiMouseButton_Middle", ImGuiMouseButton_Middle);

	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_None", ImDrawCornerFlags_None);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_TopLeft", ImDrawCornerFlags_TopLeft);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_TopRight", ImDrawCornerFlags_TopRight);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_BotLeft", ImDrawCornerFlags_BotLeft);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_BotRight", ImDrawCornerFlags_BotRight);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_Top", ImDrawCornerFlags_Top);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_Bot", ImDrawCornerFlags_Bot);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_Left", ImDrawCornerFlags_Left);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_Right", ImDrawCornerFlags_Right);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImDrawCornerFlags_All", ImDrawCornerFlags_All);

	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_None", ImGuiColorEditFlags_None);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoAlpha", ImGuiColorEditFlags_NoAlpha);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoPicker", ImGuiColorEditFlags_NoPicker);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoOptions", ImGuiColorEditFlags_NoOptions);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoSmallPreview", ImGuiColorEditFlags_NoSmallPreview);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoInputs", ImGuiColorEditFlags_NoInputs);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoTooltip", ImGuiColorEditFlags_NoTooltip);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoLabel", ImGuiColorEditFlags_NoLabel);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoSidePreview", ImGuiColorEditFlags_NoSidePreview);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoDragDrop", ImGuiColorEditFlags_NoDragDrop);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_NoBorder", ImGuiColorEditFlags_NoBorder);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_AlphaBar", ImGuiColorEditFlags_AlphaBar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_AlphaPreview", ImGuiColorEditFlags_AlphaPreview);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_AlphaPreviewHalf", ImGuiColorEditFlags_AlphaPreviewHalf);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_HDR", ImGuiColorEditFlags_HDR);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_DisplayRGB", ImGuiColorEditFlags_DisplayRGB);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_DisplayHSV", ImGuiColorEditFlags_DisplayHSV);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_DisplayHex", ImGuiColorEditFlags_DisplayHex);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_Uint8", ImGuiColorEditFlags_Uint8);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_Float", ImGuiColorEditFlags_Float);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_PickerHueBar", ImGuiColorEditFlags_PickerHueBar);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_PickerHueWheel", ImGuiColorEditFlags_PickerHueWheel);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_InputRGB", ImGuiColorEditFlags_InputRGB);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags_InputHSV", ImGuiColorEditFlags_InputHSV);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags__OptionsDefault", ImGuiColorEditFlags__OptionsDefault);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags__DisplayMask", ImGuiColorEditFlags__DisplayMask);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags__DataTypeMask", ImGuiColorEditFlags__DataTypeMask);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags__PickerMask", ImGuiColorEditFlags__PickerMask);
	DefineLuaGlobal(CH5::Lua::pLuaState, "ImGuiColorEditFlags__InputMask", ImGuiColorEditFlags__InputMask);

	return true;
}

void CH5::Lua::ReloadScripts()
{
	if (CH5::Lua::pLuaState)
	{
		CH5::Lua::Hooks::RemoveAllCallbacks();
		lua_close(CH5::Lua::pLuaState);
		CH5::Lua::pLuaState = 0;
	}
	else if (!CH5::Lua::pLuaState)
	{
		CH5::Lua::Hooks::RemoveAllCallbacks();
	}

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile("Scripts\\*.lua", &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		::MessageBox(g_HackWnd, "No Lua files found in folder Scripts...", "Crianosfera Hack v5 - BloodSharp", MB_ICONINFORMATION);
		return;
	}

	if (CH5::Lua::Initialize())
	{
		do
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::ostringstream luaFileNamePath;
				luaFileNamePath << "Scripts\\" << ffd.cFileName;
				if (luaL_dofile(CH5::Lua::pLuaState, luaFileNamePath.str().c_str()) == LUA_OK)
				{
					;
				}
				else
				{
					MessageBox(0, lua_tostring(CH5::Lua::pLuaState, -1), 0, MB_ICONERROR);
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
	}
}