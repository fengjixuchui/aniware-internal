#pragma once

enum TraceContent
{
	CONTENTS_EMPTY = 0,
	CONTENTS_SOLID = 0x1,
	CONTENTS_WINDOW = 0x2,
	CONTENTS_AUX = 0x4,
	CONTENTS_GRATE = 0x8,
	CONTENTS_SLIME = 0x10,
	CONTENTS_WATER = 0x20,
	CONTENTS_MIST = 0x40,
	CONTENTS_OPAQUE = 0x80,
	LAST_VISIBLE_CONTENTS = 0x80,
	ALL_VISIBLE_CONTENTS = ( LAST_VISIBLE_CONTENTS | ( LAST_VISIBLE_CONTENTS - 1 ) ),
	CONTENTS_TESTFOGVOLUME = 0x100,
	CONTENTS_UNUSED5 = 0x200,
	CONTENTS_UNUSED6 = 0x4000,
	CONTENTS_TEAM1 = 0x800,
	CONTENTS_TEAM2 = 0x1000,
	CONTENTS_IGNORE_NODRAW_OPAQUE = 0x2000,
	CONTENTS_MOVEABLE = 0x4000,
	CONTENTS_AREAPORTAL = 0x8000,
	CONTENTS_PLAYERCLIP = 0x10000,
	CONTENTS_MONSTERCLIP = 0x20000,
	CONTENTS_CURRENT_0 = 0x40000,
	CONTENTS_CURRENT_90 = 0x80000,
	CONTENTS_CURRENT_180 = 0x100000,
	CONTENTS_CURRENT_270 = 0x200000,
	CONTENTS_CURRENT_UP = 0x400000,
	CONTENTS_CURRENT_DOWN = 0x800000,
	CONTENTS_ORIGIN = 0x1000000,
	CONTENTS_MONSTER = 0x2000000,
	CONTENTS_DEBRIS = 0x4000000,
	CONTENTS_DETAIL = 0x8000000,
	CONTENTS_TRANSLUCENT = 0x10000000,
	CONTENTS_LADDER = 0x20000000,
	CONTENTS_HITBOX = 0x40000000,
};

enum TraceMask
{
	MASK_ALL = ( 0xFFFFFFFF ),
	MASK_SOLID = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE ),
	MASK_PLAYERSOLID = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE ),
	MASK_NPCSOLID = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE ),
	MASK_WATER = ( CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME ),
	MASK_OPAQUE = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE ),
	MASK_OPAQUE_AND_NPCS = ( MASK_OPAQUE | CONTENTS_MONSTER ),
	MASK_VISIBLE = ( MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE ),
	MASK_VISIBLE_AND_NPCS = ( MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE ),
	MASK_SHOT = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX ),
	MASK_SHOT_HULL = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE ),
	MASK_SHOT_PORTAL = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW ),
	MASK_SOLID_BRUSHONLY = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE ),
	MASK_PLAYERSOLID_BRUSHONLY = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE ),
	MASK_NPCSOLID_BRUSHONLY = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE ),
	MASK_NPCWORLDSTATIC = ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE ),
	MASK_SPLITAREAPORTAL = ( CONTENTS_WATER | CONTENTS_SLIME ),
};

enum TraceType
{
	TRACE_EVERYTHING,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct ITraceFilter
{
	VFUNC( 1, ShouldHitEntity( void* ent, unsigned int mask ), bool( __thiscall* )( void*, void*, unsigned int) )( ent, mask );
	VFUNC( 2, GetTraceType(), TraceType( __thiscall* )( void* ) )( );
};

struct TraceFilter : public ITraceFilter
{
	void* skip_ent;

	bool ShouldHitEntity(void* ent, unsigned int mask) 
	{
		return (ent != skip_ent);
	}
};

using Trace_t = void*;

struct Ray_t
{
	math::vec3_aligned_t start;
	math::vec3_aligned_t delta;
	math::vec3_aligned_t start_offset;
	math::vec3_aligned_t extents; 
	
	math::matrix3x4_t* world_axis_transform;

	bool is_ray;
	bool is_swept; 

	void initialize( const math::vec3_t& src, const math::vec3_t& dst )
	{
		delta = dst - src;
		is_swept = ( delta.length() != 0 );

		extents.x = extents.y = extents.z = 0.0f;
		is_ray = true;

		start_offset.x = start_offset.y = start_offset.z = 0.0f;
		start = src;
	}
};

struct IEngineTrace
{
	VFUNC( 4, ClipRayToEntity( const Ray_t& ray, unsigned int mask, void* ent, Trace_t trace ), 
		void( __thiscall* )( void*, const Ray_t&, unsigned int, void*, Trace_t ) )( ray, mask, ent, trace );
	
	VFUNC( 5, ClipRayToCollideable( const Ray_t& ray, unsigned int mask, void* ent, Trace_t trace ), 
		void( __thiscall* )( void*, const Ray_t&, unsigned int, void*, Trace_t ) )( ray, mask, ent, trace );
	
	VFUNC( 6, TraceRay( const Ray_t& ray, unsigned int mask, TraceFilter* skip, Trace_t trace ), 
		void( __thiscall* )( void*, const Ray_t&, unsigned int, TraceFilter*, Trace_t ) )( ray, mask, skip, trace );
};