#include "../../csgo.hpp"

std::vector< SoundInfo > logs;

void hook_handler_t::player_step( IGameEvent* e )
{
	if ( ctx::client.local && ctx::client.local->is_alive() )
	{
		if ( auto user = e->GetInt( "userid" ) )
		{
			auto walker = entity_t::get< player_t >( ctx::csgo.engine->GetPlayerForUserId( user ) );

			if ( !walker || !walker->is_alive() )
				return;

			if ( walker->get_team() != ctx::client.local->get_team() )
			{
				logs.push_back( SoundInfo{ walker->get_origin(), ctx::csgo.globals->curtime, user } );
			}
		}
	}
}
