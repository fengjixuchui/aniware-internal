#include "../../csgo.hpp"

void __fastcall hook_handler_t::override_view( REGISTERS , CViewSetup* setup )
{
	if ( ctx::client.local && ctx::client.local->is_alive( ) && !ctx::client.local->is_scoped( ) )
		setup->fov += config::get<float>( ctx::cfg.world_fov );

	hooks::get( ).orig_override_view( ecx, edx, setup );
}