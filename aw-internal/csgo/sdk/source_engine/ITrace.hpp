#pragma once

struct player_t;

#define	SURF_LIGHT 0x0001		
#define	SURF_SKY2D 0x0002		
#define	SURF_SKY 0x0004		
#define	SURF_WARP 0x0008		
#define	SURF_TRANS 0x0010
#define SURF_NOPORTAL 0x0020
#define	SURF_TRIGGER 0x0040	
#define	SURF_NODRAW 0x0080	
#define	SURF_HINT 0x0100	
#define	SURF_SKIP 0x0200	
#define SURF_NOLIGHT 0x0400	
#define SURF_BUMPLIGHT 0x0800	
#define SURF_NOSHADOWS 0x1000	
#define SURF_NODECALS 0x2000
#define SURF_NOPAINT 0x2000	
#define SURF_NOCHOP 0x4000	
#define SURF_HITBOX 0x8000

#define CHAR_TEX_ANTLION 'A'
#define CHAR_TEX_BLOODYFLESH 'B'
#define	CHAR_TEX_CONCRETE 'C'
#define CHAR_TEX_DIRT 'D'
#define CHAR_TEX_EGGSHELL 'E'
#define CHAR_TEX_FLESH 'F'
#define CHAR_TEX_GRATE 'G'
#define CHAR_TEX_ALIENFLESH	 'H'
#define CHAR_TEX_CLIP 'I'
#define CHAR_TEX_SNOW 'K'
#define CHAR_TEX_PLASTIC 'L'
#define CHAR_TEX_METAL 'M'
#define CHAR_TEX_SAND 'N'
#define CHAR_TEX_FOLIAGE 'O'
#define CHAR_TEX_COMPUTER 'P'
#define CHAR_TEX_REFLECTIVE 'R'
#define CHAR_TEX_SLOSH 'S'
#define CHAR_TEX_TILE 'T'
#define CHAR_TEX_CARDBOARD 'U'
#define CHAR_TEX_VENT 'V'
#define CHAR_TEX_WOOD 'W'
#define CHAR_TEX_GLASS 'Y'
#define CHAR_TEX_WARPSHIELD 'Z' 

#define	CONTENTS_EMPTY 0x0		
#define	CONTENTS_SOLID 0x1		
#define	CONTENTS_WINDOW 0x2	
#define	CONTENTS_AUX 0x4
#define	CONTENTS_GRATE 0x8		
#define	CONTENTS_SLIME 0x10
#define	CONTENTS_WATER 0x20
#define	CONTENTS_MIST 0x40
#define CONTENTS_OPAQUE 0x80
#define	LAST_VISIBLE_CONTENTS 0x80
#define ALL_VISIBLE_CONTENTS ( LAST_VISIBLE_CONTENTS | ( LAST_VISIBLE_CONTENTS - 1 ) )
#define CONTENTS_TESTFOGVOLUME 0x100
#define CONTENTS_UNUSED5 0x200
#define CONTENTS_UNUSED6 0x4000
#define CONTENTS_TEAM1 0x800		
#define CONTENTS_TEAM2 0x1000		
#define CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000		
#define CONTENTS_MOVEABLE 0x4000	
#define	CONTENTS_AREAPORTAL 0x8000	
#define	CONTENTS_PLAYERCLIP 0x10000
#define	CONTENTS_MONSTERCLIP 0x20000

#define	CONTENTS_CURRENT_0 0x40000
#define	CONTENTS_CURRENT_90 0x80000
#define	CONTENTS_CURRENT_180 0x100000
#define	CONTENTS_CURRENT_270 0x200000
#define	CONTENTS_CURRENT_UP 0x400000
#define	CONTENTS_CURRENT_DOWN 0x800000

#define	CONTENTS_ORIGIN 0x1000000
#define	CONTENTS_MONSTER 0x2000000
#define	CONTENTS_DEBRIS 0x4000000
#define	CONTENTS_DETAIL 0x8000000
#define	CONTENTS_TRANSLUCENT 0x10000000
#define	CONTENTS_LADDER 0x20000000
#define CONTENTS_HITBOX 0x40000000

#define	MASK_ALL ( 0xFFFFFFFF )
#define	MASK_SOLID ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE )
#define	MASK_PLAYERSOLID ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE )
#define	MASK_NPCSOLID ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE )
#define	MASK_WATER ( CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME )
#define	MASK_OPAQUE ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS ( MASK_OPAQUE | CONTENTS_MONSTER)
#define	MASK_VISIBLE ( MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE )
#define MASK_VISIBLE_AND_NPCS ( MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE )
#define	MASK_SHOT ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX )
#define MASK_SHOT_HULL ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE )
#define MASK_SHOT_PORTAL ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW )
#define MASK_SOLID_BRUSHONLY ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE )
#define MASK_PLAYERSOLID_BRUSHONLY ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE )
#define MASK_NPCSOLID_BRUSHONLY	( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE )
#define MASK_NPCWORLDSTATIC ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE ) 	

class IHandleEntity;

struct ray_t
{
	math::vec3_aligned_t start, delta, start_offset, extents;
	
	const math::matrix3x4_t* world_axis;

	bool is_ray, is_swept;

	void initialize( const math::vec3_t &src, const math::vec3_t &dst )
	{
		delta = dst - src;

		is_swept = ( delta.length_sqr( ) != 0 );

		extents.x = extents.y = extents.z = 0.0f;

		is_ray = true;

		start_offset.x = start_offset.y = start_offset.z = 0.0f;

		start = src;
	}

	void initialize( math::vec3_t &src, math::vec3_t &dst, math::vec3_t min, math::vec3_t max )
	{
		delta = dst - src;

		is_swept = ( delta.length_sqr( ) != 0 );

		extents.x = ( max.x - min.x );
		extents.y = ( max.y - min.y );
		extents.z = ( max.z - min.z );

		is_ray = false;

		start_offset.x = start_offset.y = start_offset.z = 0.0f;

		start = src + ( ( max + min ) * 0.5f );
	}
};

struct surface_t
{
	const char* name;
	short surface_props;
	unsigned short flags;
};

struct plane_t
{
	math::vec3_t normal;
	float m_dist;
	unsigned char type;
	unsigned char sign_bits;
	unsigned char pad[ 2 ];
};

struct trace_t
{
	math::vec3_t src, end;
	
	plane_t plane;

	float fraction;
	int contents;

	unsigned short display_flags;
	bool all_solid, start_solid;

	float fraction_left_solid;

	surface_t surface;

	int hitgroup;
	short physics_bone;

	player_t* entity;

	int hitbox;

	bool did_hit( ) const
	{
		return ( fraction < 1.0f );
	}

	bool did_hit_world( ) const 
	{
		return false;
	}

	bool did_hit_non_world_entity( ) const
	{
		return ( entity != NULL && !did_hit_world( ) );
	}
};

enum tracetype_t 
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
};

struct trace_filter
{
	virtual bool ShouldHitEntity( void* ent, int mask ) = 0;
	virtual tracetype_t GetTraceType( ) const = 0;
};

struct trace_filter_entity : public trace_filter
{
	bool ShouldHitEntity( void* ent, int mask )
	{
		return ( ent != skip );
	}

	tracetype_t GetTraceType( ) const 
	{
		return TRACE_EVERYTHING;
	}

	void* skip;
};

struct ITrace
{
	virtual int get_point_contents( const math::vec3_t& pos, int mask = MASK_ALL, IHandleEntity** ent = nullptr ) = 0;
	virtual int get_point_contents_world( const math::vec3_t& pos, int mask = MASK_ALL ) = 0;
	virtual int get_point_contents_collideable( void* collide, const math::vec3_t& pos ) = 0;
	virtual void clip_ray_to_entity( const ray_t& ray, unsigned int mask, player_t* ent, trace_t* trace ) = 0;
	virtual void clip_ray_to_collideable( const ray_t& ray, unsigned int mask, void* collide, trace_t* trace ) = 0;
	virtual void trace_ray( const ray_t& ray, unsigned int mask, trace_filter* filter, trace_t* trace ) = 0;
};