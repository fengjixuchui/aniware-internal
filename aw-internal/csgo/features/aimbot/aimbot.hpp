#pragma once

struct AimbotData {
	AimbotData( player_t* pl, int i, int dmg, math::vec3_t pnt, float dist ) : player( pl ), index( i ), damage( dmg ), best_point ( pnt ){};

	player_t* player;
	math::vec3_t best_point;
	int index;
	int damage;
};

namespace aimbot
{
	bool is_valid( player_t* pl );
	bool can_shoot( weapon_t* weapon );
	bool hitchance( math::vec3_t& ang, player_t* pl );
	void work();
}