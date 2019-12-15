#pragma once

struct SoundInfo
{
	SoundInfo() = default;
	SoundInfo( math::vec3_t* pos, float time, int index ) : src( pos ), source( index ){};

	int sound_id;
	void* name_handle;
	int source;
	int channel;
	int entity;
	float volume;
	float last_volume;
	float radius;
	int pitch;
	math::vec3_t* src;
	math::vec3_t* dir;
	bool update;
	bool sentence;
	bool dry_mix;
	bool speaker;
	bool special;
	bool from_server;
};

struct IEngineSound
{
	VFUNC( 19, GetActiveSounds( std::vector< SoundInfo >& sounds ), void* ( __thiscall* )( void*, std::vector< SoundInfo >& ) )( sounds );
};