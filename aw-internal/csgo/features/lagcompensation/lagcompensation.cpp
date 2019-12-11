#include "../../csgo.hpp"

std::deque< CompensationRecord > records[ 65 ];

BestRecord best_record;

ConVars cvars;

namespace lagcompensation
{
	float lerp_time()
	{
		const float ratio = std::clamp( cvars.interp_ratio->GetFloat(), cvars.min_interp_ratio->GetFloat(), cvars.max_interp_ratio->GetFloat() );

		if ( ratio )
		{
			return std::max( cvars.interp->GetFloat(), ( ratio / ( ( cvars.max_update_rate ) ? cvars.max_update_rate->GetFloat() : cvars.update_rate->GetFloat() ) ) );
		}
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

		const float delta_time = std::clamp( net_channel->GetLatency( FLOW_OUTGOING ) + lerp_time(),
			0.f, cvars.max_unlag->GetFloat() ) - ( ctx::csgo.globals->curtime - time );

		if ( delta_time )
		{
			return std::fabsf( delta_time ) <= 0.2f;
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

	bool get_player_record( player_t* pl, CompensationRecord& record )
	{
		if ( !pl->SetupBones( record.matrix, 128, 0x7FF00, ctx::csgo.globals->curtime ) )
			return false;

		record.head = pl->get_hitbox_pos( HITBOX_HEAD );
		record.view = pl->get_eye_angles();
		record.simulation_time = pl->get_simtime();
		
		if ( valid_tick( record.simulation_time ) && record.matrix->m_matrix )
			return true;
	}

	void initialize()
	{
		if ( !records->empty() )
			records->clear();
		
		if ( cvars.initialize )
		{
			cvars.update_rate = ctx::csgo.cvar->FindVar( "cl_updaterate" );
			cvars.max_update_rate = ctx::csgo.cvar->FindVar( "sv_maxupdaterate" );
			cvars.interp = ctx::csgo.cvar->FindVar( "cl_interp" );
			cvars.interp_ratio = ctx::csgo.cvar->FindVar( "cl_interp_ratio" );
			cvars.min_interp_ratio = ctx::csgo.cvar->FindVar( "sv_client_min_interp_ratio" );
			cvars.max_interp_ratio = ctx::csgo.cvar->FindVar( "sv_client_max_interp_ratio" );
			cvars.max_unlag = ctx::csgo.cvar->FindVar( "sv_maxunlag" );

			cvars.initialize = false;
		}
	}

	void clear_variables( player_t* pl )
	{
		const auto vars_map = reinterpret_cast< uintptr_t> ( pl ) + 0x24;
		const auto vars_count = *reinterpret_cast< int* >( static_cast< uintptr_t >( vars_map ) + 0x14 );

		for ( auto i = 0; i < vars_count; i++ )
		{
			*reinterpret_cast< uintptr_t* >( *reinterpret_cast< uintptr_t* >( vars_map ) + i * 0xC ) = 0;
		}
	}

	void update()
	{
		if ( !config::get< bool >( ctx::cfg.lagcompensation ) || !ctx::csgo.engine->IsInGame() || !ctx::client.local )
		{
			if ( !records->empty() )
			{
				records->clear();
			}

			return;
		}

		initialize();

		game::for_every_player( []( player_t * pl ) -> bool {
			const int index = pl->Index();

			if ( !is_valid( pl ) )
				records[ index ].clear();

			if ( records[ index ].size() && ( records[ index ].front().simulation_time == pl->get_simtime() ) )
				return false;

			clear_variables( pl );

			CompensationRecord record{};

			if ( get_player_record( pl, record ) )
				records[ index ].push_front( record );

			while ( records[ index ].size() <= 3 && ( records[ index ].size() > static_cast< size_t >( time_to_ticks ( config::get< float >( ctx::cfg.lagcompensation_ms ) / 1000.f ) ) ) )
				records[ index ].pop_back();

			return false;
		}, { game::ENEMY_ONLY } );

	}

	void work()
	{
		if ( !config::get< bool >( ctx::cfg.lagcompensation ) || config::get< bool >( ctx::cfg.aim_lagcompensation ) )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		if ( !ctx::client.cmd->buttons.has_flag( IN_ATTACK ) )
			return;

		static float best_fov { 255.f };

		game::for_every_player( []( player_t * pl ) -> bool {
			if ( !is_valid( pl ) )
				return false;

			const auto pos = pl->get_hitbox_pos( HITBOX_HEAD );
			const auto ang = math::calc_angle( ctx::client.local->get_eye_pos(), pos );
				
			if ( !ang.valid() )
				return false;

			const auto fov = std::hypotf( ang.x, ang.y );

			if ( fov < best_fov )
			{
				best_fov = fov;

				best_record.target = pl;
				best_record.head = pos;
			}

			return false;
		}, { game::ENEMY_ONLY } );
	}
}