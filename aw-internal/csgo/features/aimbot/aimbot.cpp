#include "../../csgo.hpp"

AimbotData data;

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

		Ray_t ray;
		Trace_t trace;

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
		
		const float server_time = static_cast< float >( weapon->tickbase() * ctx::csgo.globals->interval_per_tick );

		if ( weapon->get_next_primary_attack() > server_time)
			return false;

		if ( weapon->get_definition_index() == WEAPON_REVOLVER && weapon->ready_time() > server_time )
			return false;

		return true;
	}

	/*
	bool hitchance( math::vec3_t& ang, player_t* pl )
	{
		static int traces_hit = 0;
		math::vec3_t forward, right, up;

		const auto eye_position = ctx::client.local->get_eye_pos();
		math::angle_vectors( ang, &forward, &right, &up );

		const auto weapon = entity_t::get< weapon_t >( ctx::client.local->get_weapon_handle() );

		if ( !weapon || weapon->get_ammo() <= 0 )
			return false;

		weapon->update_accuracy();

		const float weapon_spread = weapon->get_spread();
		const float weapon_cone = weapon->get_inaccuracy();

		const auto get_bullet_location = [&]( int seed ) 
		{
			if ( auto module_handle = GetModuleHandleA( "vstdlib.dll" ) )
			{
				reinterpret_cast< void*( * )( unsigned int ) >( GetProcAddress( module_handle, "RandomSeed" ) )( seed );
			}

			const float rand[ 4 ] = {
				math::random(0.f, 1.f),
				math::random(0.f, 2.f * M_PI),
				math::random(0.f, 1.f),
				math::random(0.f, 2.f * M_PI)
			};

			const float generated_spread = rand[ 0 ] * weapon_spread;
			const float generated_cone = rand[ 2 ] * weapon_cone;

			const math::vec3_t spread = {
				std::cos( rand[ 1 ] ) * generated_spread + std::cos( rand[ 3 ] ) * generated_cone,
				std::sin( rand[ 1 ] ) * generated_spread + std::sin( rand[ 3 ] ) * generated_cone,
				0.f
			};

			return math::vec3_t( forward + right * -spread.x + up * -spread.y );
		};

		for ( auto i = 1; i <= 256; i++ )
		{
			math::vec3_t spread_angle;
			math::vec3_t bullet_end;

			math::vector_angles( get_bullet_location( i ), spread_angle );
			math::angle_vectors( ang - ( spread_angle - ang ), bullet_end );

			Ray_t ray;
			Trace_t trace;
			
			ray.initialize( eye_position, eye_position + bullet_end * weapon->get_weapon_info()->range );
			ctx::csgo.enginetrace->ClipRayToEntity( ray, MASK_SHOT, pl, &trace );

			if ( trace.entity == pl )
				++traces_hit;

			if ( traces_hit >= static_cast< int >( config::get< float >( ctx::cfg.aim_hitchance ) * 2.56f ) )
				return true;

		}

		return false;
	}
	*/

	void select_target()
	{
		for ( auto i = 0; i < HITBOX_MAX; ++i )
		{
			math::vec3_t point = data.pl->get_hitbox_pos( i );

			if ( !is_visible( data.pl, point ) )
					continue;

			calculated_points.push_back( point );
		}

		data.src = ctx::client.local->get_eye_pos();

		if ( calculated_points.empty() )
			return;

		static float best_distance = std::numeric_limits< float >::max();

		for ( auto &point : calculated_points )
		{
			const float distance = math::vec3_t{ data.src - point }.length();

			if ( distance < best_distance )
			{
				best_distance = distance;
				data.dst = point;
			}
		}
	}

	void choose_angles()
	{
		if ( !data.pl || !data.wp || !data.src.valid() || !data.dst.valid() )
			return;

		auto angle = math::calc_angle( data.src, data.dst );

		if ( angle.valid() )
		{
			angle = {
				std::clamp< float >( angle.x, -89.f, 89.f ),
				std::clamp< float >( angle.y, -180.f, 180.f ),
				std::clamp< float >( angle.z, 0.f, 0.f )
			};

			data.wp->update_accuracy();
			
			ctx::client.cmd->viewangles = angle - ( ctx::client.local->get_punch_angle() * 2.0f );

			if( config::get< bool >( ctx::cfg.aim_shoot ) )
				ctx::client.cmd->buttons.add_flag( IN_ATTACK );

			if( !config::get< bool >( ctx::cfg.aim_silent ) )
				ctx::csgo.engine->SetViewAngles( ctx::client.cmd->viewangles );
		}
	}

	void restore_players()
	{
		data.src = math::vec3_t{};
		data.dst = math::vec3_t{};

		data.wp = nullptr;
		data.pl = nullptr;

		if ( !calculated_points.empty() )
			calculated_points.clear();
	}

	void work()
	{
		if( !config::get< bool >( ctx::cfg.aim_enable ) )
			return;

		if ( !ctx::client.cmd )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		data.wp = entity_t::get< weapon_t >( ctx::client.local->get_weapon_handle() );

		if ( !can_shoot( data.wp ) )
			return;

		game::for_every_player( []( player_t * pl ) -> bool {
			if ( !is_valid( pl ) )
				return false;

			data.pl = pl;

			select_target();
			choose_angles();
			restore_players();

			return false;
		}, ( config::get< bool >( ctx::cfg.aim_friendly ) ? game::NO_FLAG : game::ENEMY_ONLY ) );
	}
}