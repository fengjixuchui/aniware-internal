#pragma once

struct IVDebugOverlay
{
	VFUNC( 13, WorldToScreen( const math::vec3_t& src, math::vec3_t& dst ), int( __thiscall* )( void*, const math::vec3_t&, math::vec3_t& ) )( src, dst );
};