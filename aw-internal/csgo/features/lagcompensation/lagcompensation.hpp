#pragma once

struct CompensationRecord
{
	float simulation_time;

	math::vec3_t view;
	math::vec3_t head;

	math::matrix3x4_t matrix[ 128 ];
};

struct BestRecord
{
	CompensationRecord record;
	player_t* target;

	math::vec3_t head;
};

struct ConVars
{
	bool initialize = true;

	ConVar* update_rate;
	ConVar* max_update_rate;
	ConVar* interp;
	ConVar* interp_ratio;
	ConVar* min_interp_ratio;
	ConVar* max_interp_ratio;
	ConVar* max_unlag;
};

extern std::deque< CompensationRecord > records[65];

extern ConVars cvars;

namespace lagcompensation
{
	bool valid_tick( float time );
	void update( );
}