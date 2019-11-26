#include "../../csgo.hpp"

enum HitmarkerSound {
	NOSOUND,
	SWITCH_PRESS,
	PAINTCAN_HARD
};

void hook_handler_t::player_hurt( IGameEvent* e )
{
	if ( config::get<bool>( ctx::cfg.hitmarker_enable ) && ctx::client.local && ctx::client.local->is_alive() )
	{
		if ( auto attacker_id = ctx::csgo.engine->GetPlayerForUserId( e->GetInt( "attacker" ) ) != ctx::csgo.engine->GetLocalPlayer() )
			return;

		if ( config::get<bool>( ctx::cfg.hitmarker_tint ) )
			ctx::client.local->get_health_boost_time() = ctx::csgo.globals->curtime + 0.25f;
		
		switch ( config::get<int>( ctx::cfg.hitmarker_sound ) )
		{
		case HitmarkerSound::NOSOUND:
			break;
		case HitmarkerSound::SWITCH_PRESS:
			ctx::csgo.engine->ClientCmd_Unrestricted( "play buttons/arena_switch_press_02.wav" );
			break;
		case HitmarkerSound::PAINTCAN_HARD:
			ctx::csgo.engine->ClientCmd_Unrestricted( "play physics/metal/paintcan_impact_hard3.wav" );
			break;
		}
	}
}
