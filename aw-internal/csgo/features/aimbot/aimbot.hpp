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
	void work();
}