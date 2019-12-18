#pragma once

struct AimbotData
{
	AimbotData() = default;
	AimbotData( player_t* p ) : pl( p ){};

	math::matrix3x4_t bones[ 128 ];
	std::vector< math::vec3_t > points;

	player_t* pl;
};

namespace aimbot
{
	void work();
}