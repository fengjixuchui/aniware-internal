#pragma once

struct SoundInfo
{
	SoundInfo( ) = default;
	SoundInfo( const math::vec3_t p, float t, int i ) : src( p ), time( t ), userid( i ){};

	math::vec3_t src;
	float time;
	int userid;
};

struct IEngineSound
{
	VFUNC( 19, GetActiveSounds( void* sounds ), void* ( __thiscall* )( void*, void* ) )( sounds );
};