#pragma once

namespace autowall
{
	void trace_line( const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, void* skip, Trace_t* trace );
	void clip_trace_to_players( const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, TraceFilter* filter, Trace_t* trace );
	bool is_breakable( entity_t* ent );
}