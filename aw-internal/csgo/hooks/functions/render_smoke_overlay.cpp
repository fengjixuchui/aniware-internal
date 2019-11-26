#include "../../csgo.hpp"

void __fastcall hook_handler_t::render_smoke_overlay( REGISTERS, bool render_overlay )
{
	if ( !config::get<bool>( ctx::cfg.draw_smoke ) )
		return hooks::get().orig_render_smoke_overlay( ecx, edx, render_overlay );
}