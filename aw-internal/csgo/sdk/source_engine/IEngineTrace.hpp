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
	ALL_VISIBLE_CONTENTS = 0xFF,
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

enum SurfProps {
	SURF_LIGHT = 0X1,
	SURF_SKY2D = 0X2,
	SURF_SKY = 0X4,
	SURF_WARP = 0X8,
	SURF_TRANS = 0X10,
	SURF_NOPORTAL = 0X20,
	SURF_TRIGGER = 0X40,
	SURF_NODRAW = 0X80,
	SURF_HINT = 0X100,
	SURF_SKIP = 0X200,
	SURF_NOLIGHT = 0X400,
	SURF_BUMPLIGHT = 0X800,
	SURF_NOSHADOWS = 0X1000,
	SURF_NODECALS = 0X2000,
	SURF_NOPAINT = SURF_NODECALS,
	SURF_NOCHOP = 0X4000,
	SURF_HITBOX = 0X8000
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

struct Surface_t
{
	const char* name;
	short props;
	unsigned short flags;
};

struct Plane_t
{
	math::vec3_t normal;
	float distance;
	unsigned char type;
	unsigned char bits;
	unsigned char pad_0x2[ 2 ];
};

struct player_t;

struct ITraceFilter
{
	VFUNC( 1, ShouldHitEntity( player_t* ent, unsigned int mask ), bool( __thiscall* )( void*, player_t*, unsigned int ) )( ent, mask );
	VFUNC( 2, GetTraceType(), TraceType( __thiscall* )( void* ) )( );
};

struct TraceFilter : public ITraceFilter
{
	void* skip;

	bool should_hit_entity( player_t* ent, unsigned int mask ) 
	{
		return ( ent != skip );
	}
};

struct Trace
{
	bool did_hit() const {
		return fraction < 1.f;
	}

	math::vec3_t start;
	math::vec3_t end;

	Plane_t plane;
	float fraction;
	int contents;
	unsigned short flags;
	bool all_solid;
	bool start_solid;
	float fraction_left_solid;
	Surface_t surface;

	int hitgroup;
	short physics_bone;
	player_t* entity;
	int hitbox;
};

struct Ray_t
{
	math::vec3_aligned_t start;
	math::vec3_aligned_t delta;
	math::vec3_aligned_t start_offset;
	math::vec3_aligned_t extents; 
	
	const math::matrix3x4_t* world_axis_transform;

	bool is_ray;
	bool is_swept; 

	void initialize( const math::vec3_t& src, const math::vec3_t& dst )
	{
		delta = dst - src;
		start = src;

		is_ray = true;
		is_swept = ( delta.length() != 0 );

		start_offset.x = start_offset.y = start_offset.z = 0.0f;
		extents.x = extents.y = extents.z = 0.0f;
	}
};

struct IEngineTrace
{
	virtual int GetPointContents( const math::vec3_t& pos, int mask = MASK_ALL, void** ent = nullptr ) = 0;
	virtual int GetPointContentsWorld( const math::vec3_t& pos, int mask = MASK_ALL) = 0;
	virtual int GetPointContentsCollideable( void* collide, const math::vec3_t& pos ) = 0;
	virtual void ClipRayToEntity( const Ray_t& ray, unsigned int mask, player_t* ent, Trace* trace ) = 0;
	virtual void ClipRayToCollideable( const Ray_t& ray, unsigned int mask, void* collide, Trace* trace ) = 0;
	virtual void TraceRay( const Ray_t& ray, unsigned int mask, TraceFilter* filter, Trace* trace ) = 0;
};