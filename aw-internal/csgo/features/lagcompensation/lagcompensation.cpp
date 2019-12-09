#include "../../csgo.hpp"

std::deque< CompensationRecord > records[ 65 ];

ConVars cvars;

namespace lagcompensation
{
	float lerp_time()
	{
		const auto ratio = std::clamp(cvars.interp_ratio->GetFloat(), cvars.min_interp_ratio->GetFloat(), cvars.max_interp_ratio->GetFloat());
		return std::max(cvars.interp->GetFloat(), (ratio / ((cvars.max_update_rate) ? cvars.max_update_rate->GetFloat() : cvars.update_rate->GetFloat())));
	}

	int time_to_ticks( float time )
	{
		return static_cast< int >( ( 0.5f + static_cast< float >( time ) / ctx::csgo.globals->interval_per_tick ) );
	}

	bool valid_tick ( float time )
	{
		const auto net_channel = ctx::csgo.engine->GetNetChannelInfo();
		
		if ( !net_channel )
			return false;

		const float delta = std::clamp( net_channel->GetLatency( FLOW_OUTGOING ) + lerp_time(),
			0.f, cvars.max_unlag->GetFloat() ) - (ctx::csgo.globals->curtime - time);

		return std::fabsf( delta ) <= 0.2f;
	}

	void initialize()
	{
		records->clear();

		if ( cvars.interp )
		{
			cvars.update_rate = ctx::csgo.cvar->FindVar( "cl_updaterate" );
			cvars.max_update_rate = ctx::csgo.cvar->FindVar( "sv_maxupdaterate" );
			cvars.interp = ctx::csgo.cvar->FindVar( "cl_interp" );
			cvars.interp_ratio = ctx::csgo.cvar->FindVar( "cl_interp_ratio" );
			cvars.min_interp_ratio = ctx::csgo.cvar->FindVar( "sv_client_min_interp_ratio" );
			cvars.max_interp_ratio = ctx::csgo.cvar->FindVar( "sv_client_max_interp_ratio" );
			cvars.max_unlag = ctx::csgo.cvar->FindVar( "sv_maxunlag" );
		}
	}

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

	void update()
	{
		if ( !config::get< bool >( ctx::cfg.lagcompensation ) || !ctx::client.local || !ctx::client.local->is_alive() )
		{
			if ( !records->empty() )
				records->clear();

			return;
		}

		game::for_every_player( []( player_t * pl ) -> bool {
			const auto index = pl->EntIndex();

			if ( !is_valid( pl ) )
			{
				records[ index ].clear();
				
				return false;
			}

			if ( records[ index ].size() && ( records[ index ].front().simulation_time == pl->get_simtime() ) )
				return false;

			return false;
		}, { game::ENEMY_ONLY } );
	}

	void work()
	{

	}
}