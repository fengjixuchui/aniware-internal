#include "../../csgo.hpp"

enum StrafeType {
	RAGE,
	LEGIT,
	SALTED
};

namespace movement
{
	void bunnyhop()
	{
		if ( !config::get<bool>( ctx::cfg.movement_bhop ) )
			return;

		if ( !ctx::client.cmd )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		const auto move_type = ctx::client.local->get_move_type();
		if ( move_type == MOVETYPE_LADDER ||
			 move_type == MOVETYPE_NOCLIP ||
			 move_type == MOVETYPE_OBSERVER )
			return;

		if ( ctx::client.local->get_water_level() >= 2 )
			return;

		if ( !ctx::client.local->get_flags().has_flag( FL_ONGROUND ) )
			ctx::client.cmd->buttons.remove_flag( IN_JUMP );
	}

	void strafe()
	{
		if ( !config::get<bool>( ctx::cfg.movement_strafe_enable ) )
			return;

		if ( !ctx::client.cmd )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		const auto move_type = ctx::client.local->get_move_type();
		if ( move_type == MOVETYPE_LADDER ||
			 move_type == MOVETYPE_NOCLIP ||
			 move_type == MOVETYPE_OBSERVER )
			return;

		if ( ctx::client.local->get_water_level() >= 2 )
			return;

		if ( ctx::client.local->get_flags().has_flag( FL_ONGROUND ) && !ctx::client.cmd->buttons.has_flag( IN_JUMP ) )
			return;
			
		math::angle_t viewangles;
		ctx::csgo.engine->GetViewAngles( viewangles );

		static bool side_switch = false;
		side_switch = !side_switch;

		ctx::client.cmd->forwardmove = 0.f;
		ctx::client.cmd->sidemove = side_switch ? 450.f : -450.f;

		float velocity_yaw = ctx::client.local->get_velocity().y;
		float rotation = std::clamp< float >( math::rad2deg( std::atan2( 15.f, ctx::client.local->get_velocity().length_2d() ) ), 0.f, 90.f );

		float delta { velocity_yaw - viewangles.y };
		math::normalize_angle( delta );

		float ideal_yaw_rotation = ( side_switch ? rotation : -rotation ) + ( std::fabs( delta ) < 5.f ? velocity_yaw : viewangles.y );

		float cos_rot = std::cos( math::deg2rad( viewangles.y - ideal_yaw_rotation ) );
		float sin_rot = std::sin( math::deg2rad( viewangles.y - ideal_yaw_rotation ) );

		const float forwardmove = ctx::client.cmd->forwardmove;
		const float sidemove = ctx::client.cmd->sidemove;

		switch ( config::get<int>( ctx::cfg.movement_strafe_type ) )
		{
		case RAGE:
			break;
		case LEGIT:
			cos_rot /= 5;
			sin_rot /= 5;
			break;
		case SALTED:
			sin_rot *= static_cast< float >( std::sin( ctx::csgo.globals->tickcount ) * 0.15 );
			break;
		}

		ctx::client.cmd->forwardmove = (cos_rot * forwardmove) - (sin_rot * sidemove);
		ctx::client.cmd->sidemove = (sin_rot * forwardmove) + (cos_rot * sidemove);
	}
}