#include "../../csgo.hpp"

bool __fastcall hook_handler_t::do_screen_effects( REGISTERS, const CViewSetup* setup ) {
	if( !ctx::client.local )
		return hooks::get( ).orig_do_screen_effects( ecx, edx, setup );

	return hooks::get( ).orig_do_screen_effects( ecx, edx, setup );
}