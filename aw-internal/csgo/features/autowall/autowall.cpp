#include "../../csgo.hpp"

namespace autowall
{
	void trace_line(const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, void* skip, Trace_t* trace)
	{
		if ( !src.valid() || !dst.valid() )
			return;

		Ray_t ray;
		TraceFilter filter;

		ray.initialize( src, dst );
		filter.skip = skip;
		
		ctx::csgo.enginetrace->TraceRay( ray, mask, &filter, trace );
	}

	void get_bullet_type( float& range, float& dist, std::string type, bool penetration )
	{
		if ( penetration )
		{
			range = 35.0;
			dist = 3000.0;
		}
		else
		{
			if ( type == "BULLET_PLAYER_338MAG" )
			{
				range = 45.0;
				dist = 8000.0;
			}

			if (type == "BULLET_PLAYER_762MM" )
			{
				range = 39.0;
				dist = 5000.0;
			}

			if ( type == "BULLET_PLAYER_556MM" ||  type == "BULLET_PLAYER_556MM_SMALL" ||  type == "BULLET_PLAYER_556MM_BOX" )
			{
				range = 35.0;
				dist = 4000.0;
			}

			if ( type == "BULLET_PLAYER_57MM" )
			{
				range = 30.0;
				dist = 2000.0;
			}

			if ( type == "BULLET_PLAYER_50AE" )
			{
				range = 30.0;
				dist = 1000.0;
			}

			if ( type == "BULLET_PLAYER_357SIG" ||  type == "BULLET_PLAYER_357SIG_SMALL" ||  type == "BULLET_PLAYER_357SIG_P250" ||  type == "BULLET_PLAYER_357SIG_MIN" )
			{
				range = 25.0;
				dist = 800.0;
			}

			if ( type == "BULLET_PLAYER_9MM" )
			{
				range = 21.0;
				dist = 800.0;
			}

			if ( type == "BULLET_PLAYER_45ACP" )
			{
				range = 15.0;
				dist = 500.0;
			}

			if ( type == "BULLET_PLAYER_BUCKSHOT" )
			{
				range = 0.0;
				dist = 0.0;
			}
		}
	}
}