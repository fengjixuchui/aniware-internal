#pragma once

namespace autowall
{
	void trace_line( const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, void* skip, Trace_t* trace );
	void get_bullet_type( float& range, float& dist, std::string type, bool penetration );
}