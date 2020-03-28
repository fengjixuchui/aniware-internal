#include "../../csgo.hpp"

void __fastcall hook_handler_t::trace_ray( REGISTERS, const Ray& ray, unsigned int mask, TraceFilter filter, Trace* trace )
{
	hooks::get( ).orig_trace_ray( ecx, edx, ray, mask, filter, trace );

	trace->surface.flags.add_flag( SURF_SKY );
}