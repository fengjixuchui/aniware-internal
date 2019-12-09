#pragma once

struct CompensationRecord
{
	float simulation_time;

	math::vec3_t head;
	math::matrix3x4_t matrix[ 128 ];
};

struct ConVars
{
	ConVar* update_rate;
	ConVar* max_update_rate;
	ConVar* interp;
	ConVar* interp_ratio;
	ConVar* min_interp_ratio;
	ConVar* max_interp_ratio;
	ConVar* max_unlag;
};

namespace lagcompensation
{
	void update();
	void work();
}