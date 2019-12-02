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

	bool hitchance( math::angle_t ang, player_t* pl )
	{
		if ( !ctx::client.cmd )
			return false;

		if ( !ctx::client.local->is_alive() )
			return false;

		int traces_hit = 0;
		static math::vec3_t forward, right, up;

		const auto eye_position = ctx::client.local->get_eye_pos();
		const auto punch_angle = ctx::client.local->get_punch_angle();

		math::angle_vectors( math::angle_t{ ang + punch_angle * 2.f }, forward, right, up );
		
		if ( auto weapon = entity_t::get<weapon_t>( ctx::client.local->get_weapon_handle() ) )
		{
			weapon->update_accuracy();
			
			const float weapon_spread = weapon->get_spread();
			const float weapon_cone = weapon->get_inaccuracy();

			const auto get_bullet_location = [ & ]( int seed ) {
				if ( auto module_handle = GetModuleHandleA( "vstdlib.dll" ) )
				{
					reinterpret_cast< void*( * )( unsigned int )>( GetProcAddress( module_handle, "RandomSeed" ) )( seed );
				}

				const float rand[ 4 ] = { 
					math::random( 0.f, 1.f ), 	  
					math::random( 0.f, float{ 2.f * M_PI } ), 
					math::random( 0.f, 1.f ), 
					math::random( 0.f, float{ 2.f * M_PI } ), 
				};

				const float generated_spread = weapon_spread * rand[ 0 ];
				const float generated_cone = weapon_cone * 2.f;

				const math::vec3_t spread = { 
					std::cos( rand[ 1 ] ) * generated_spread + std::cos( rand[ 3 ] ) * generated_cone, 
					std::sin( rand[ 1 ] ) * generated_spread + std::sin( rand[ 3 ] ) * generated_cone,
					0.f
				};

				return math::vec3_t{ forward + right * -spread.x + up * -spread.y }.normalize();
			};

			for ( auto i = 1; i <= 256; i++ )
			{
				static math::angle_t spread_angle;
				static math::vec3_t predicted_bullet_end;

				math::vector_angles( get_bullet_location( i ), spread_angle );
				math::angle_vectors( ang - ( spread_angle - ang ), predicted_bullet_end );

				Trace_t trace;
				Ray_t ray;

				ray.initialize( eye_position, { eye_position + predicted_bullet_end * weapon->get_wpn_data()->flWeaponRange } );
				ctx::csgo.enginetrace->ClipRayToEntity( ray, MASK_SHOT, pl, &trace );

				if ( reinterpret_cast< player_t* >( trace.entity ) == pl )
					++traces_hit;

				if ( traces_hit >= static_cast< int >( config::get< float >( ctx::cfg.aim_hitchance ) * 2.56f ) )
					return true;
			}
		}

		return false;
	}

	void work()
	{
		if( !config::get< bool >( ctx::cfg.aim_enable ) )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		game::for_every_player( []( player_t * pl ) -> bool {
			if ( !is_valid( pl ) )
				return false;

			auto weapon = entity_t::get<weapon_t>( ctx::client.local->get_weapon_handle() );
			
			weapon->update_accuracy();

			const auto aim_punch = ctx::client.local->get_punch_angle() * ctx::csgo.cvar->FindVar( "weapon_recoil_scale" )->GetFloat();

			auto ang = math::calc_angle( ctx::client.local->get_eye_pos(), pl->get_hitbox_pos( HITBOX_HEAD ) ) - aim_punch;

			ang.x = std::clamp( ang.x, -89.0f, 89.0f );
			ang.y = std::clamp( ang.y, -180.0f, 180.0f );
			ang.z = 0;

			ctx::client.cmd->viewangles = ang;

			if ( !config::get< bool >( ctx::cfg.aim_silent ) )
				ctx::csgo.engine->SetViewAngles( ctx::client.cmd->viewangles );

			if ( hitchance( ang + aim_punch, pl ) )
				ctx::client.cmd->buttons.add_flag( IN_ATTACK );

			return false;
		}, ( config::get< bool >( ctx::cfg.aim_friendly ) ? game::NO_FLAG : game::ENEMY_ONLY ) );
	}
}