#include "../../csgo.hpp"

void __fastcall hook_handler_t::paint( REGISTERS, int mode )
{
	hooks::get().orig_paint( ecx, edx, mode );

	if ( mode & PAINT_UIPANELS )
	{
		ctx::csgo.surface->StartDrawing();
		{
			if ( input::get_key_info( VK_XBUTTON1 ).is_pressed() )
				config::save();

			if ( input::get_key_info( VK_XBUTTON2 ).is_pressed() )
				config::load();

			render::init();

			players::render();
			menu::render();

			input::update_mouse();
		}
		ctx::csgo.surface->FinishDrawing();
	}
}
