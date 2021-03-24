#pragma once

#include "CH5_External.h"

typedef struct player_info_s_nuevo
{
	int userid;
	char userinfo[256];
	char name[32];
	int spectator;
	int ping;
	int packet_loss;
	char model[64];
	int topcolor;
	int bottomcolor;
	int renderframe;
	int gaitsequence;
	float gaitframe;
	float gaityaw;
	vec3_t prevgaitorigin;
	customization_t customdata;
	char hashedcdkey[16];
	unsigned __int64 m_nSteamID;
}player_info_t_nuevo;

extern int Cstrike_SequenceInfo[];

enum
{
	SEQUENCE_IDLE = 0,
	SEQUENCE_SHOOT = 1,
	SEQUENCE_RELOAD = 2,
	SEQUENCE_DIE = 4,
	SEQUENCE_THROW = 8,
	SEQUENCE_ARM_C4 = 16,
	SEQUENCE_SHIELD = 32,
	SEQUENCE_SHIELD_SIDE = 64
};

class CH5Player
{
	public:
		void UpdateByIndex(int id);

		bool CanRenderAtScreen();
		bool IsAlive();

		ImColor GetPlayerColor();

		void RenderName();
		void RenderBox();
		void RenderModelName();
		void RenderDistance();
		void RenderSequence();
		void RenderLatency();

		int GetTeam();

	//private:
		int m_iIndex;
		int m_iTeam;
		bool m_bAlive;
		Vector m_vOrigin;
		Vector m_vTop2D;
		Vector m_vBottom2D;
		float m_flDistance;
		cl_entity_t m_clEntity;
		model_t m_mdlModel;
		player_info_t_nuevo m_stPlayerInfo;
		//extra_player_info_t m_stExtraPlayerInfo;
};

#define CH5_MAX_PLAYERS 33
extern CH5Player vPlayers[CH5_MAX_PLAYERS];