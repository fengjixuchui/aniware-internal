#include "../../csgo.hpp"

enum HealthType {
	NOHEALTH,
	BAR,
	NUMBER
};

enum BoxType {
	NOBOX,
	RECTANGLE,
	CORNERS
};

namespace players
{
	void render()
	{
		if ( !ctx::csgo.engine->IsInGame() || !ctx::csgo.engine->IsConnected() )
			return;

		game::for_every_player( []( player_t * pl ) -> bool {
			math::vec4_t bbox {};
			if ( !pl->get_bbox( bbox ) )
				return false;

			if ( config::get<bool>( ctx::cfg.extrasensory_name ) )
			{
				auto player_info = pl->get_player_info();
				render::text( render::fonts::m_main, { bbox.x + bbox.w * 0.5f, bbox.y - 7 }, { 255, 255, 255 }, { render::fonts::FONT_RIGHT | render::fonts::FONT_CENTER_Y }, player_info.name );
			}

			const int player_health = pl->get_health();

			col_t health_color { static_cast< int >( 255 - player_health * 2.55 ), static_cast< int >( player_health * 2.55 ), 0, 255 };
			
			player_health > 100 ? health_color = col_t{ 0, 255, 0 } : col_t {};

			switch ( config::get<int>( ctx::cfg.extrasensory_health_type ) )
			{
			case HealthType::NOHEALTH:
				break;
			case HealthType::BAR:
				render::rect_filled( { bbox.x - 6, bbox.y - 1 }, { 3, bbox.z + 2 }, { 0, 0, 0, 255 } );
				render::rect_filled( { bbox.x - 5, bbox.y + ( bbox.z - bbox.z * ( std::clamp< int >( player_health, 0, 100.f) / 100.f) ) }, { 1, bbox.z * ( std::clamp< int >( player_health, 0, 100 ) / 100.f ) - ( player_health >= 100 ? 0 : -1 ) }, health_color );
				break;
			case HealthType::NUMBER:
				render::text( render::fonts::m_main, { bbox.x + bbox.w * 0.5f, bbox.y - 7 }, { 255, 255, 255 }, { render::fonts::FONT_CENTER_Y }, fmt::format( " [ {:d} ]", pl->get_health() ) );
				break;
			}

			switch ( config::get<int>( ctx::cfg.extrasensory_box_type ) )
			{
			case BoxType::NOBOX:
				break;
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

			return false;
		}, { game::ENEMY_ONLY } );
	}
}