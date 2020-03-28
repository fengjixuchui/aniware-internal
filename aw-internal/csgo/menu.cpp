#include "csgo.hpp"

namespace menu
{
	float get_curtime( )
	{
		return ctx::csgo.globals->curtime;
	}

	float get_frametime( )
	{
		return ctx::csgo.globals->frametime;
	}

	void text( const math::vec2_t& pos, const col_t& col, std::string_view text )
	{
		render::text( render::fonts::m_menu, pos, col, 0, text );
	}

	math::vec2_t text_size( std::string_view text )
	{
		return render::text_size( render::fonts::m_menu, text );
	}

	void clip( const math::vec2_t& pos, const math::vec2_t& size, bool override_clip = false )
	{
		return render::clip( pos, size, override_clip );
	}

	void init( )
	{
		gui::setup_helper(
			get_curtime,
			get_frametime,
			render::rect_filled,
			render::rect,
			text,
			text_size,
			render::reset_clip,
			clip
		);

		gui::set_toggle_key( VK_INSERT );
		gui::set_hold_key( VK_MENU );

		gui::set_name( "-> aniware_internal" );

		if ( auto tab1 = gui::add_tab( "ragebot" ) )
		{
			tab1->add( std::make_shared<gui::controls::c_separator>( ".aim" ) );
			{
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "enable", &config::get_item( ctx::cfg.aim_enable ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "friendly", &config::get_item( ctx::cfg.aim_friendly ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "body only", &config::get_item( ctx::cfg.aim_body ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "auto shoot", &config::get_item( ctx::cfg.aim_shoot ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "silent", &config::get_item( ctx::cfg.aim_silent ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "lag compensation", &config::get_item( ctx::cfg.aim_lagcompensation ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "recoil compensation", &config::get_item( ctx::cfg.aim_recoilcompensation ) ) );
				tab1->add( std::make_shared<gui::controls::c_slider>( "hitchance", &config::get_item( ctx::cfg.aim_hitchance ), 0.f, 100.f ) );
			}

			tab1->add( std::make_shared<gui::controls::c_separator>( ".lag compensation" ) );
			{
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "enable", &config::get_item( ctx::cfg.lagcompensation ) ) );
				tab1->add( std::make_shared<gui::controls::c_checkbox>( "show", &config::get_item( ctx::cfg.lagcompensation_show ) ) );
				tab1->add( std::make_shared<gui::controls::c_slider>( "ms", &config::get_item( ctx::cfg.lagcompensation_ms ), 0.f, 200.f ) );
			}
		}

		if ( auto tab2 = gui::add_tab( "visuals" ) )
		{
			tab2->add( std::make_shared<gui::controls::c_separator>( ".extrasensory" ) );
			{
				tab2->add( std::make_shared<gui::controls::c_checkbox>( "fog", &config::get_item( ctx::cfg.draw_fog ) ) );
				tab2->add( std::make_shared<gui::controls::c_checkbox>( "name", &config::get_item( ctx::cfg.extrasensory_name ) ) );
				tab2->add( std::make_shared<gui::controls::c_combo>( "health type", &config::get_item( ctx::cfg.extrasensory_health_type ), std::vector<std::string_view>{ "disabled", "bar", "number" } ) );
				tab2->add( std::make_shared<gui::controls::c_combo>( "box type", &config::get_item( ctx::cfg.extrasensory_box_type ), std::vector<std::string_view>{ "disabled", "rectangle", "corners" } ) );
			}

			tab2->add( std::make_shared<gui::controls::c_separator>( ".cosmetic" ) );
			{
				tab2->add( std::make_shared<gui::controls::c_slider>( "viewmodel fov", &config::get_item( ctx::cfg.view_fov ), 0.f, 100.f ) );
				tab2->add( std::make_shared<gui::controls::c_slider>( "world fov", &config::get_item( ctx::cfg.world_fov ), 0.f, 100.f ) );
			}
		}

		if ( auto tab3 = gui::add_tab( "misc" ) )
		{
			tab3->add( std::make_shared<gui::controls::c_separator>( ".hitmarker" ) );
			{
				tab3->add( std::make_shared<gui::controls::c_checkbox>( "enable", &config::get_item( ctx::cfg.hitmarker_enable ) ) );
				tab3->add( std::make_shared<gui::controls::c_checkbox>( "tint", &config::get_item( ctx::cfg.hitmarker_tint ) ) );
				tab3->add( std::make_shared<gui::controls::c_combo>( "sound", &config::get_item( ctx::cfg.hitmarker_sound ), std::vector<std::string_view>{ "disabled", "switch", "paintcan" } ) );
			}

			tab3->add( std::make_shared<gui::controls::c_separator>( ".movement" ) );
			{
				tab3->add( std::make_shared<gui::controls::c_checkbox>( "auto jump", &config::get_item( ctx::cfg.movement_bhop ) ) );
				tab3->add( std::make_shared<gui::controls::c_checkbox>( "auto strafe", &config::get_item( ctx::cfg.movement_strafe_enable ) ) );
				tab3->add( std::make_shared<gui::controls::c_combo>( "strafe type", &config::get_item( ctx::cfg.movement_strafe_type ), std::vector<std::string_view>{ "rage", "legit" } ) );	
			}			
		}
	}

	void render( )
	{
		gui::set_pos( render::get_screen_size( ) * 0.5f );
		gui::set_size( { 200, 14 } );

		gui::setup_style( );

		ctx::csgo.surface->m_bClippingEnabled( ) = true;

		gui::render( );

		ctx::csgo.surface->m_bClippingEnabled( ) = false;
	}
}