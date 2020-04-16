#include "../../csgo.hpp"
#include "../lagcompensation/lagcompensation.hpp"

enum HealthType
{
	NOHEALTH,
	NUMBER,
	BAR,
};

enum BoxType
{
	NOBOX,
	RECTANGLE,
	CORNERS
};

namespace players
{
	void name( math::vec4_t bbox, player_t* pl )
	{
		auto player_info = pl->get_player_info( );

		render::text( render::fonts::m_main, { bbox.x + bbox.w * 0.5f, bbox.y - 7 }, { 255, 255, 255 }, { render::fonts::FONT_RIGHT | render::fonts::FONT_CENTER_Y }, player_info.name );
	}

	void dot( player_t* pl )
	{
		math::vec3_t dst{};
		
		col_t color{};

		if ( static_cast< bool >( ctx::csgo.debugoverlay->WorldToScreen( pl->get_eye_pos( ), dst ) != 1 ) )
		{
			ctx::client.local->can_see_pos( pl, pl->get_eye_pos( ) ) ? color = col_t{ 100, 255, 100 } : color = col_t{ 255, 100, 100 };

			render::text( render::fonts::m_main, { dst.x, dst.y }, color, { render::fonts::FONT_CENTER_X | render::fonts::FONT_CENTER_Y }, "x" );
		}
	}

	void ticks( player_t* pl )
	{
		for ( auto record : records[ pl->Index( ) ] )
		{
			if ( !record.matrix || !lagcompensation::valid_tick( record.simulation_time ) )
				continue;

			math::vec3_t dst{};

			if ( static_cast< bool >( ctx::csgo.debugoverlay->WorldToScreen( record.head, dst ) != 1 ) )
			{
				render::text(render::fonts::m_main, { dst.x, dst.y }, { 255, 255, 255 }, { render::fonts::FONT_CENTER_X | render::fonts::FONT_CENTER_Y }, "x");
			}
		}
	}

	void health( math::vec4_t bbox, player_t* pl )
	{
		const auto get_health_color = []( player_t* pl, int health )
		{
			col_t health_color { static_cast< int >( 255 - health * 2.55f ), static_cast< int >( health * 2.55f ), 0, 255 };
			{
				( health > 100 ) ? health_color = col_t{ 0, 255, 0 } : col_t{};
			}

			return health_color;
		};

		const auto player_health = pl->get_health( );

		const auto player_color = get_health_color( pl, player_health );

		switch ( config::get< int >( ctx::cfg.extrasensory_health_type ) )
		{
		case HealthType::NUMBER:
			render::text( render::fonts::m_main, { bbox.x + bbox.w * 0.5f, bbox.y - 7 }, { 255, 255, 255 }, { render::fonts::FONT_CENTER_Y }, fmt::format( " [ {:d} ]", pl->get_health( ) ) );
			break;

		case HealthType::BAR:
			render::rect_filled( { bbox.x - 6, bbox.y - 1 }, { 3, bbox.z + 2 }, { 0, 0, 0, 255 } );
			render::rect_filled( { bbox.x - 5, bbox.y + ( bbox.z - bbox.z * ( std::clamp< int >( player_health, 0, 100.f) / 100.f) ) }, { 1, bbox.z * ( std::clamp< int >( player_health, 0, 100 ) / 100.f ) - ( player_health >= 100 ? 0 : -1 ) }, player_color );
			break;
		}
	}

	void box( math::vec4_t bbox, player_t* pl )
	{
		switch ( config::get< int >( ctx::cfg.extrasensory_box_type ) )
		{
		case BoxType::RECTANGLE:
			render::rect( { bbox.x - 1, bbox.y - 1 }, { bbox.w + 2, bbox.z + 2 }, { 10, 10, 10 } );
			render::rect( { bbox.x + 1, bbox.y + 1 }, { bbox.w - 2, bbox.z - 2 }, { 10, 10, 10 } );
			render::rect( { bbox.x, bbox.y }, { bbox.w, bbox.z }, { 255, 255, 255 } );
			break;
		case BoxType::CORNERS:
			render::corner( { bbox.x - 1, bbox.y - 1 }, { bbox.w + 2, bbox.z + 2 }, { 10, 10, 10 } );
			render::corner( { bbox.x + 1, bbox.y + 1 }, { bbox.w - 2, bbox.z - 2 }, { 10, 10, 10 } );
			render::corner( { bbox.x, bbox.y }, { bbox.w, bbox.z }, { 255, 255, 255 } );
			break;
		}
	}

	void render( )
	{
		if ( !ctx::csgo.engine->IsInGame( ) || !ctx::csgo.engine->IsConnected( ) )
			return;

		game::for_every_player( []( player_t * pl ) -> bool {
			math::vec4_t bbox {};
			if ( !pl->get_bbox( bbox ) )
				return false;

			if ( config::get< bool >( ctx::cfg.extrasensory_name ) )
				name( bbox, pl );

			if ( config::get< bool >( ctx::cfg.extrasensory_dot ) )
				dot( pl );

			if ( config::get< bool >( ctx::cfg.lagcompensation_show ) )
				ticks( pl );

			if ( config::get< int >( ctx::cfg.extrasensory_health_type ) )
				health( bbox, pl );

			if ( config::get< int >( ctx::cfg.extrasensory_health_type ) )
				box( bbox, pl );

			return false;
		}, { game::ENEMY_ONLY } );
	}
}