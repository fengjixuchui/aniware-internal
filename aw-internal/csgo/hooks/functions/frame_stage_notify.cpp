#include "../../csgo.hpp"

void __fastcall hook_handler_t::frame_stage_notify( REGISTERS, client_frame_stage_t stage )
{
	switch ( stage )
	{
		case FRAME_RENDER_START:
			ctx::csgo.engine->ClientCmd_Unrestricted( "say FRAME_RENDER_START" );
			break;
	}

	return hooks::get().orig_frame_stage_notify( ecx, edx, stage );
}