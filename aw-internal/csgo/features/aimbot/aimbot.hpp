#pragma once

struct AimbotData
{
	math::vec3_t src;
	math::vec3_t dst;

	weapon_t* wp;
	player_t* pl;
};

namespace aimbot
{
	bool is_valid( player_t* pl );
	bool can_shoot( weapon_t* weapon );
	bool hitchance( math::vec3_t& ang, player_t* pl );
	void work();
}