#pragma once

struct AutowallData {
	AutowallData(const math::vec3_t s, const math::vec3_t d, player_t* t, player_t* f) : src( s ), dst( d ){};

	math::vec3_t src;
	math::vec3_t dst;
};

namespace autowall
{
	void trace_line( const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, void* skip, Trace_t* trace );
	void clip_trace_to_players( const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, TraceFilter* filter, Trace_t* trace );
	bool is_breakable( entity_t* ent );
}