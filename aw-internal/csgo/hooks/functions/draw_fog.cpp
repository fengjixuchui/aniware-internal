#include "../../csgo.hpp"

bool __fastcall hook_handler_t::draw_fog( REGISTERS )
{
	return config::get<bool>( ctx::cfg.draw_fog );
}