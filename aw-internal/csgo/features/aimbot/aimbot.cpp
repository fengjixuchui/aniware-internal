#include "../../csgo.hpp"

namespace aimbot
{
	bool hitchance( math::angle_t ang, player_t* pl )
	{
		if ( !ctx::client.cmd )
			return false;

		if ( !ctx::client.local->is_alive() )
			return false;

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
				if ( reinterpret_cast< void*( * )( unsigned int )>( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "RandomSeed" ) )( seed ) )
					return math::vec3_t{};
				
				const float rand[ 4 ] = { math::random( 0.f, 1.f ), 
										  math::random( 0.f, float{ 2.f * M_PI } ), 
										  math::random( 0.f, 1.f ), 
										  math::random( 0.f, float{ 2.f * M_PI } ), 
				};

				const float generated_spread = weapon_spread * rand[ 0 ];
				const float generated_cone = weapon_cone * 2.f;

				const math::vec3_t spread = { std::cos( rand[ 1 ] )* generated_spread + std::cos( rand[ 3 ] ) * generated_cone, 
											  std::sin( rand[ 1 ] )* generated_spread + std::sin( rand[ 3 ] ) * generated_cone,
											  0.f
				};

				return math::vec3_t{ forward + right * -spread.x + up * -spread.y }.normalize();
			};

			for ( auto i = 1; i <= 256; i++ ) {
				static math::angle_t spread_angle;
				static math::vec3_t predicted_bullet_end;

				math::vector_angles( get_bullet_location( i ), spread_angle );
				math::angle_vectors( ang - ( spread_angle - ang ), predicted_bullet_end );

				/*
				todo: finish hitchance
				trace trace
				if hitchance
				ret true

				*/
			}
		}

		return false;
	}
}