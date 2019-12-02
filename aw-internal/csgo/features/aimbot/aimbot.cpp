#include "../../csgo.hpp"

namespace aimbot
{
	bool is_valid( player_t* pl )
	{
		if ( !pl || pl->IsDormant() )
			return false;

		if( pl == ctx::client.local )
			return false;

		if ( !pl->is_alive() || pl->get_flags().has_flag( FL_FROZEN ) )
			return false;

		if ( pl->is_immune() )
			return false;

		return true;
	}

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

		const auto get_bullet_location = [&]( int seed ) {
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

			const float hitchance = config::get< float >( ctx::cfg.aim_hitchance );

			if ( traces_hit >= static_cast< int >( hitchance * 2.56f ) )
				return true;

		}

		return false;
	}

	void work()
	{
		if( !config::get< bool >( ctx::cfg.aim_enable ) )
			return;

		if ( !ctx::client.cmd )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		game::for_every_player( []( player_t * pl ) -> bool {
			if ( !is_valid( pl ) )
				return false;

			hitchance( ctx::client.cmd->viewangles, pl ) ? ctx::client.cmd->buttons.add_flag( IN_ATTACK ) : ctx::client.cmd->buttons.remove_flag( IN_ATTACK );

			return false;
		}, ( config::get< bool >( ctx::cfg.aim_friendly ) ? game::NO_FLAG : game::ENEMY_ONLY ) );
	}
}