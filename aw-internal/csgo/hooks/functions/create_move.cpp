#include "../../csgo.hpp"

bool __fastcall hook_handler_t::create_move( REGISTERS, float flInputSampleTime, CUserCmd* cmd )
{
	if ( !cmd || !cmd->command_number )
		return hooks::get().orig_create_move( ecx, edx, flInputSampleTime, cmd );

	ctx::client.cmd = cmd;
	ctx::client.local = entity_t::get<player_t>( ctx::csgo.engine->GetLocalPlayer() );

	INIT_STACKFRAME();
	stack_frame.previous();

	auto send_packet = stack_frame.get_var< bool* >( -0x1C );

	aimbot::work();

	movement::bunnyhop();
	movement::strafe();

	return false;
}