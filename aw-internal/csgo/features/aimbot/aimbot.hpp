#pragma once

struct AimbotData
{
	AimbotData() = default;
	AimbotData( player_t* p ) : pl( p ){};

	std::vector< math::vec3_t > points;
	player_t* pl;
};

namespace aimbot
{
	void work();
}