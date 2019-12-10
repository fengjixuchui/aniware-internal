#include "../../csgo.hpp"

std::vector< math::vec3_t > calculated_points;

namespace aimbot
{
	bool is_valid( player_t* pl )
	{
		if ( pl->IsDormant() )
			return false;

		if( pl == ctx::client.local )
			return false;

		if ( !pl->is_alive() || pl->get_flags().has_flag( FL_FROZEN ) )
			return false;

		if ( pl->is_immune() )
			return false;

		return true;
	}

	bool is_visible( player_t* pl, const math::vec3_t& dst )
	{
		if ( !dst.valid() )
			return false;

		Ray ray;
		Trace trace;

		TraceFilter filter;
		filter.skip = ctx::client.local;

		ray.initialize( ctx::client.local->get_eye_pos(), dst );

		ctx::csgo.enginetrace->TraceRay( ray, ( MASK_SHOT | CONTENTS_GRATE ), &filter, &trace );

		return trace.entity == pl || trace.fraction > 0.97f;
	}

	bool can_shoot( weapon_t* weapon )
	{
		if ( !weapon || weapon->get_ammo() <= 0 )
			return false;
		
		if ( weapon->get_next_primary_attack() > ctx::csgo.globals->curtime )
			return false;

		if ( weapon->get_definition_index() == WEAPON_REVOLVER && weapon->ready_time() > ctx::csgo.globals->curtime )
			return false;

		return true;
	}
}