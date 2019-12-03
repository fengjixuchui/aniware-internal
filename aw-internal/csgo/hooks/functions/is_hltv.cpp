#include "../../csgo.hpp"

bool __fastcall hook_handler_t::is_hltv( REGISTERS )
{
	static const auto accumulate_layers_call = static_cast<void*>( mem::find_ida_sig( "client_panorama.dll", "84 C0 75 0D F6 87" ) );
	static const auto setupvelocity_call = static_cast<void*>( mem::find_ida_sig( "client_panorama.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0" ) );

	auto original = hooks::get().orig_is_hltv( ecx, edx );

	if ( !ctx::csgo.engine->IsInGame() || !ctx::client.local )
		return original;

	if ( _ReturnAddress() == accumulate_layers_call || _ReturnAddress() == setupvelocity_call )
		return true;

	return original;
}