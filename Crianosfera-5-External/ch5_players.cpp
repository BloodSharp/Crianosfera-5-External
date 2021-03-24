#include "CH5_External.h"

CH5Player vPlayers[CH5_MAX_PLAYERS];

int Cstrike_SequenceInfo[] =
{
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, // 0..9   
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 10..19 
	1,	2,	0,	1,	1,	2,	0,	1,	1,	2, // 20..29 
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 30..39 
	1,	2,	0,	1,	2,	0,	1,	2,	0,	1, // 40..49 
	2,	0,	1,	2,	0,	0,	0,	8,	0,	8, // 50..59 
	0, 16,	0, 16,	0,	0,	1,	1,	2,	0, // 60..69 
	1,	1,	2,	0,	1,	0,	1,	0,	1,	2, // 70..79 
	0,	1,	2, 	32, 40, 32, 40, 32, 32, 32, // 80..89
	33, 64, 33, 34, 64, 65, 34, 32, 32, 4, // 90..99
	4,	4,	4,	4,	4,	4,	4,	4,	4,	4, // 100..109
	4                                      	// 110
};

void CH5Player::UpdateByIndex(int id)
{
	this->m_iIndex = id;
	this->m_mdlModel.name[0] = 0;
	this->m_iTeam = 0;
	if (CH5::Offsets::dwProcessId && CH5::Offsets::dwHwDllBase && CH5::Offsets::dwClientDllBase && CH5::Offsets::dwClEntityT)
	{
		HANDLE hGameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CH5::Offsets::dwProcessId);
		if (hGameHandle != INVALID_HANDLE_VALUE)
		{
			SIZE_T uiBytesRead = 0;
			DWORD dwEntityBaseAddress = 0;
			DWORD dwExtraPlayerInfoBaseAddress = 0;
			ReadProcessMemory(hGameHandle, (LPCVOID)(CH5::Offsets::dwHwDllBase + CH5::Offsets::dwClEntityT), (LPVOID)&dwEntityBaseAddress, sizeof(dwEntityBaseAddress), &uiBytesRead);
			if (uiBytesRead == sizeof(dwEntityBaseAddress))
			{
				ReadProcessMemory(hGameHandle, (LPCVOID)(dwEntityBaseAddress + sizeof(cl_entity_t) * this->m_iIndex), (LPVOID)&this->m_clEntity, sizeof(this->m_clEntity), &uiBytesRead);
				if (this->m_clEntity.model)
				{
					ReadProcessMemory(hGameHandle, (LPCVOID)(this->m_clEntity.model), (LPVOID)&this->m_mdlModel, sizeof(this->m_mdlModel), &uiBytesRead);
					if (uiBytesRead == sizeof(this->m_mdlModel))
					{
						if (
							strstr(this->m_mdlModel.name, "gign9") ||
							strstr(this->m_mdlModel.name, "gsg9") ||
							strstr(this->m_mdlModel.name, "sas") ||
							strstr(this->m_mdlModel.name, "urban") ||
							strstr(this->m_mdlModel.name, "vip"))
							this->m_iTeam = 2;
						else if (
							strstr(this->m_mdlModel.name, "arctic") ||
							strstr(this->m_mdlModel.name, "guerilla") ||
							strstr(this->m_mdlModel.name, "leet") ||
							strstr(this->m_mdlModel.name, "terror"))
							this->m_iTeam = 1;
					}
				}
			}
			ReadProcessMemory(hGameHandle, (LPCVOID)((CH5::Offsets::dwHwDllBase + CH5::Offsets::dwPlayerInfoS) + sizeof(player_info_s_nuevo) * (this->m_iIndex - 1)), (LPVOID)&this->m_stPlayerInfo, sizeof(player_info_t_nuevo), &uiBytesRead);
			CloseHandle(hGameHandle);
		}
	}
}

bool CH5Player::CanRenderAtScreen()
{
	Vector vOriginTop, vOriginBottom;
	//VectorCopy(this->m_stPlayerInfo.prevgaitorigin, this->m_vOrigin);
	VectorCopy(this->m_clEntity.origin, this->m_vOrigin);
	VectorCopy(this->m_vOrigin, vOriginTop);
	VectorCopy(this->m_vOrigin, vOriginBottom);
	vOriginTop.z += 20;
	vOriginBottom.z -= 40;
	if( !this->m_vOrigin.IsZero() &&
		CH5::Engine::WorldToScreen(vOriginTop, (float*)&this->m_vTop2D) &&
		CH5::Engine::WorldToScreen(vOriginBottom, (float*)&this->m_vBottom2D))
		return true;
	return false;
}

bool CH5Player::IsAlive()
{
	return (
		this->m_mdlModel.name[0] &&
		!(this->m_clEntity.curstate.effects & EF_NODRAW) &&
		this->m_clEntity.player &&
		!this->m_clEntity.curstate.spectator &&
		this->m_clEntity.curstate.solid &&
		!(this->m_clEntity.curstate.messagenum < vPlayers[CH5::LocalPlayer::GetIndex()].m_clEntity.curstate.messagenum));
}

ImColor CH5Player::GetPlayerColor()
{
	if (this->m_iIndex == CH5::Aimbot::Target)
		return CH5::Settings::ESP::Colors::Aimbot;
	if (this->m_iTeam == 2)
		return CH5::Settings::ESP::Colors::CounterTerrorist;
	else if (this->m_iTeam == 1)
		return CH5::Settings::ESP::Colors::Terrorist;
	return ImColor(255, 255, 255);
}

void CH5Player::RenderName()
{
	if (CH5::Settings::ESP::Name)
	{
		ImVec2 vTextSize = ImGui::CalcTextSize(this->m_stPlayerInfo.name, NULL, true);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vTop2D.x - (vTextSize.x / 2.0f) - 1, this->m_vTop2D.y - ImGui::GetFontSize() -1),
			ImColor(0, 0, 0), this->m_stPlayerInfo.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vTop2D.x - (vTextSize.x / 2.0f) - 1, this->m_vTop2D.y - ImGui::GetFontSize() + 1),
			ImColor(0, 0, 0), this->m_stPlayerInfo.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vTop2D.x - (vTextSize.x / 2.0f) + 1, this->m_vTop2D.y - ImGui::GetFontSize() -1),
			ImColor(0, 0, 0), this->m_stPlayerInfo.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vTop2D.x - (vTextSize.x / 2.0f) + 1, this->m_vTop2D.y - ImGui::GetFontSize() + 1),
			ImColor(0,0,0), this->m_stPlayerInfo.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vTop2D.x - (vTextSize.x / 2.0f), this->m_vTop2D.y - ImGui::GetFontSize()),
			this->GetPlayerColor(), this->m_stPlayerInfo.name);
	}
}

void CH5Player::RenderBox()
{
	if (CH5::Settings::ESP::Box)
	{
		float iBoxHeight = std::abs(this->m_vTop2D.y - this->m_vBottom2D.y);
		float iBoxWidth = iBoxHeight / 1.8f;
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->m_vTop2D.x - iBoxWidth / 2 - 1, this->m_vTop2D.y - 1),
			ImVec2(this->m_vTop2D.x + iBoxWidth / 2 - 1, this->m_vTop2D.y + iBoxHeight - 1),
			ImColor(0, 0, 0), 0.0f, ImDrawCornerFlags_All, 2.0f);
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->m_vTop2D.x - iBoxWidth / 2 - 1, this->m_vTop2D.y + 1),
			ImVec2(this->m_vTop2D.x + iBoxWidth / 2 - 1, this->m_vTop2D.y + iBoxHeight + 1),
			ImColor(0, 0, 0), 0.0f, ImDrawCornerFlags_All, 2.0f);
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->m_vTop2D.x - iBoxWidth / 2 + 1, this->m_vTop2D.y - 1),
			ImVec2(this->m_vTop2D.x + iBoxWidth / 2 + 1, this->m_vTop2D.y + iBoxHeight - 1),
			ImColor(0, 0, 0), 0.0f, ImDrawCornerFlags_All, 2.0f);
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->m_vTop2D.x - iBoxWidth / 2 + 1, this->m_vTop2D.y + 1),
			ImVec2(this->m_vTop2D.x + iBoxWidth / 2 + 1, this->m_vTop2D.y + iBoxHeight + 1),
			ImColor(0, 0, 0), 0.0f, ImDrawCornerFlags_All, 2.0f);
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->m_vTop2D.x - iBoxWidth / 2, this->m_vTop2D.y),
			ImVec2(this->m_vTop2D.x + iBoxWidth / 2, this->m_vTop2D.y + iBoxHeight),
			this->GetPlayerColor(), 0.0f, ImDrawCornerFlags_All, 2.0f);
		//Box(vEye2D.x - iBoxWidth / 2, vEye2D.y, iBoxWidth, iBoxHeight, color);
	}
}

void CH5Player::RenderDistance()
{
	if (CH5::Settings::ESP::Distance)
	{
		Vector vDifference = this->m_vOrigin - CH5::LocalPlayer::GetEyePosition();
		int iDistance = int(vDifference.Length() / 22.0f);
		std::ostringstream distanceMessage;
		distanceMessage << "Distance: " << iDistance;
		ImVec2 vTextSize = ImGui::CalcTextSize(distanceMessage.str().c_str(), NULL, true);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 8 + ImGui::GetFontSize() * 3/*space offset*/ - 1),
			ImColor(0, 0, 0), distanceMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 8 + ImGui::GetFontSize() * 3/*space offset*/ + 1),
			ImColor(0, 0, 0), distanceMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 8 + ImGui::GetFontSize() * 3/*space offset*/ - 1),
			ImColor(0, 0, 0), distanceMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 8 + ImGui::GetFontSize() * 3/*space offset*/ + 1),
			ImColor(0, 0, 0), distanceMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f), this->m_vBottom2D.y + 8 + ImGui::GetFontSize() * 3/*space offset*/),
			this->GetPlayerColor(), distanceMessage.str().c_str());
	}
}

void CH5Player::RenderModelName()
{
	if (CH5::Settings::ESP::ModelName)
	{
		ImVec2 vTextSize = ImGui::CalcTextSize(this->m_mdlModel.name, NULL, true);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 2/*space offset*/ - 1),
			ImColor(0, 0, 0), this->m_mdlModel.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 2/*space offset*/ + 1),
			ImColor(0, 0, 0), this->m_mdlModel.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 2/*space offset*/ - 1),
			ImColor(0, 0, 0), this->m_mdlModel.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 2/*space offset*/ + 1),
			ImColor(0, 0, 0), this->m_mdlModel.name);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f), this->m_vBottom2D.y + 2/*space offset*/),
			this->GetPlayerColor(), this->m_mdlModel.name);
	}
}

void CH5Player::RenderSequence()
{
	char* pszSequenceInformation = 0;
	int seqInfo = Cstrike_SequenceInfo[this->m_clEntity.curstate.sequence];
	if (seqInfo & SEQUENCE_ARM_C4)
		pszSequenceInformation = (char*)"- ARMING C4 -";
	else if (seqInfo & SEQUENCE_RELOAD)
		pszSequenceInformation = (char*)"- RELOADING -";
	else if (seqInfo & SEQUENCE_THROW)
		pszSequenceInformation = (char*)"- THROWING GRENADE -";
	else if (seqInfo & SEQUENCE_SHIELD)
		pszSequenceInformation = (char*)"- SHIELDED -";
	else if (seqInfo & SEQUENCE_SHIELD_SIDE)
		pszSequenceInformation = (char*)"- SHIELD TO SIDE -";
	else if (seqInfo & SEQUENCE_SHOOT)
		pszSequenceInformation = (char*)"- SHOOTING -";
	else if (seqInfo & SEQUENCE_IDLE)
		pszSequenceInformation = (char*)"- IDLE -";
	if (pszSequenceInformation)
	{
		ImVec2 vTextSize = ImGui::CalcTextSize(pszSequenceInformation, NULL, true);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 4 + ImGui::GetFontSize()/*space offset*/ - 1),
			ImColor(0, 0, 0), pszSequenceInformation);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 4 + ImGui::GetFontSize()/*space offset*/ + 1),
			ImColor(0, 0, 0), pszSequenceInformation);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 4 + ImGui::GetFontSize()/*space offset*/ - 1),
			ImColor(0, 0, 0), pszSequenceInformation);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 4 + ImGui::GetFontSize()/*space offset*/ + 1),
			ImColor(0, 0, 0), pszSequenceInformation);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f), this->m_vBottom2D.y + 4 + ImGui::GetFontSize()/*space offset*/),
			this->GetPlayerColor(), pszSequenceInformation);
	}
}

void CH5Player::RenderLatency()
{
	if (CH5::Settings::ESP::Latency)
	{
		std::ostringstream pingMessage;
		pingMessage << "Ping: " << this->m_stPlayerInfo.ping;

		ImVec2 vTextSize = ImGui::CalcTextSize(pingMessage.str().c_str(), NULL, true);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 6 + ImGui::GetFontSize()*2/*space offset*/ - 1),
			ImColor(0, 0, 0), pingMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) - 1, this->m_vBottom2D.y + 6 + ImGui::GetFontSize()*2/*space offset*/ + 1),
			ImColor(0, 0, 0), pingMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 6 + ImGui::GetFontSize()*2/*space offset*/ - 1),
			ImColor(0, 0, 0), pingMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f) + 1, this->m_vBottom2D.y + 6 + ImGui::GetFontSize()*2/*space offset*/ + 1),
			ImColor(0, 0, 0), pingMessage.str().c_str());
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(this->m_vBottom2D.x - (vTextSize.x / 2.0f), this->m_vBottom2D.y + 6 + ImGui::GetFontSize()*2/*space offset*/),
			this->GetPlayerColor(), pingMessage.str().c_str());
	}
}

int CH5Player::GetTeam()
{
	return this->m_iTeam;
}