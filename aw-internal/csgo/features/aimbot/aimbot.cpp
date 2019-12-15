#include "../../csgo.hpp"

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

	bool can_shoot( weapon_t* weapon )
	{
		if ( !weapon || weapon->get_ammo() <= 0 )
			return false;
		
		if ( weapon->get_next_primary_attack() > ctx::csgo.globals->curtime )
			return false;

		if ( weapon->get_definition_index() == WEAPON_REVOLVER && weapon->ready_time() > ctx::csgo.globals->curtime )
			return false;

		return true;
	}

	void work()
	{
		if ( !config::get< bool >( ctx::cfg.aim_enable ) )
			return;

		if ( !ctx::client.cmd )
			return;

		if ( !ctx::client.local || !ctx::client.local->is_alive() )
			return;

		const auto weapon = entity_t::get< weapon_t >( ctx::client.local->get_weapon_handle() );

		if ( !can_shoot( weapon ) )
			return;

		game::for_every_player( []( player_t * pl ) -> bool {
			if ( !is_valid( pl ) )
				return false;

			const auto position = pl->get_hitbox_pos( HITBOX_HEAD );
			
			if ( !position.valid() )
				return false;

			if ( ctx::client.cmd->buttons.has_flag( IN_ATTACK ) )
			{
				ctx::client.cmd->viewangles = math::calc_angle( ctx::client.local->get_eye_pos(), position );

				if (  !config::get< bool >( ctx::cfg.aim_silent ) )
				{
					ctx::csgo.engine->SetViewAngles( ctx::client.cmd->viewangles );
				}
			}

			return false;
		}, ( config::get< bool >( ctx::cfg.aim_friendly ) ? game::NO_FLAG : game::ENEMY_ONLY ) );
	}
}