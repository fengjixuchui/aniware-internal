#pragma once

struct AimbotData {
	AimbotData( player_t* pl, int i, int dmg, math::vec3_t pnt, float dist ) : player( pl ), index( i ), damage( dmg ), best_point ( pnt ), distance( dist ){};
		
	player_t* player;
	int index;
	int damage;
	math::vec3_t best_point;
	float distance;
};

std::vector< ::AimbotData > aim_data;
player_t* aim_last_target;
int aim_shots_fired;

namespace aimbot
{
	bool is_valid( player_t* pl );
	bool hitchance( math::vec3_t& ang, player_t* pl );
	void work();
}