#pragma once

#include "CH5_External.h"

namespace CH5
{
	namespace Lua
	{
		extern lua_State* pLuaState;
		bool Initialize();
		void ReloadScripts();

		enum CH5_CALLBACK_TYPE
		{
			CH5_CALLBACK_AT_RENDERING_INTERFACE,
			CH5_CALLBACK_AT_RENDERING_BACKGROUND,
			CH5_CALLBACK_AT_RENDERING_MENU,
			CH5_CALLBACK_AT_UPDATE,
			CH5_CALLBACK_AT_LOAD_SETTINGS,
			CH5_CALLBACK_AT_SAVE_SETTINGS,

			CH5_CALLBACK_ALL_CALLBACKS
		};

		namespace Hooks
		{
			void RegisterCallBack(UINT type, luabridge::LuaRef f);
			//void RegisterCallBack(const char* type, luabridge::LuaRef f);
			std::vector<luabridge::LuaRef> GetCallbacks(CH5_CALLBACK_TYPE type);
			bool HasHook(CH5_CALLBACK_TYPE type);
			void RemoveAllCallbacks();
			//extern std::unordered_map<CH5_CALLBACK_TYPE, std::vector<luabridge::LuaRef>> Callbacks;
			extern std::unordered_map<UINT, std::vector<luabridge::LuaRef>> Callbacks;
		}

		namespace ImGui
		{
			bool Begin(const char* szTitle, ImGuiWindowFlags flags);
			void End();
			void Text(const char* szText);
			bool Checkbox(const char* szText, bool bCurrentValue);
			void SameLine();
			void NewLine();
			bool TreeNode(const char* szTitle, bool bDefaultOpen);
			void TreePop();
			bool SmallButton(const char* szText);
			int  Combo(const char* szText, int iCurrentValue, const char* szOptions);
			void BeginGroup();
			void EndGroup();
			bool BeginTabBar(const char* szId);
			void EndTabBar();
			bool BeginTabItem(const char* szTitle);
			void EndTabItem();
			int  GetFramesPerSecond();
			void PushItemWidth(int width);
			void PopItemWidth();
			bool IsItemClicked(ImGuiMouseButton mouse_button);
			ImColor ColorEdit4(const char* szColor, ImColor& color, int flags);

			namespace Render
			{
				void AddRect(ImVec2& v1, ImVec2& v2, ImColor& color, float rounding, int corners, float thickness);
				void AddLine(ImVec2& v1, ImVec2& v2, ImColor& color, float thickness);
				void AddText(ImVec2& position, ImColor& color, const char* szText);
				void AddRectFilled(ImVec2& v1, ImVec2& v2, ImColor& color, float rounding, int corners);
			}
		}

		namespace Utils
		{
			bool ConvertToScreen(Vector& in);
			Vector GetLastConvertedToScreenVector();
			extern Vector vLastConvertedVector;

			// Files
			int LoadInteger(const char* szFile, const char* szValue);
			float LoadFloat(const char* szFile, const char* szValue);
			bool LoadBoolean(const char* szFile, const char* szValue);
			ImColor LoadColor(const char* szFile, const char* szValue);
			void SaveInteger(const char* szFile, const char* szValue, int iValue);
			void SaveFloat(const char* szFile, const char* szValue, float fValue);
			void SaveBoolean(const char* szFile, const char* szValue, bool bValue);
			void SaveColor(const char* szFile, const char* szValue, ImColor& color);
			int LoadIntegerInSection(const char* szFile, const char* szSection, const char* szValue);
			float LoadFloatInSection(const char* szFile, const char* szSection, const char* szValue);
			bool LoadBooleanInSection(const char* szFile, const char* szSection, const char* szValue);
			ImColor LoadColorInSection(const char* szFile, const char* szSection, const char* szValue);
			void SaveIntegerInSection(const char* szFile, const char* szSection, const char* szValue, int iValue);
			void SaveFloatInSection(const char* szFile, const char* szSection, const char* szValue, float fValue);
			void SaveBooleanInSection(const char* szFile, const char* szSection, const char* szValue, bool bValue);
			void SaveColorInSection(const char* szFile, const char* szSection, const char* szValue, ImColor& color);
		}

		namespace LocalPlayer
		{
			int GetIndex();
			int GetTeam();
			Vector GetEyePosition();
			int GetFlags();
			bool IsAlive();
			Vector GetForwardVector();
			int GetButtons();
			void SetViewAngles(Vector& newangles);
		}

		namespace EntityManager
		{
			int GetTeam(int index);
			Vector GetOrigin(int index);
			std::string GetName(int index);
			std::string GetModelName(int index);
			int GetDistance(int index);
			float GetActualDistance(int index);
			int GetPing(int index);
			bool IsAlive(int index);
			int GetSequence(int index);
		}
	}
}