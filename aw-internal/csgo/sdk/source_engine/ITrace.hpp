#pragma once

enum content_masks_t : unsigned int
{
	contents_empty = 0x0,
	contents_solid = 0x1,
	contents_window = 0x2,
	contents_aux = 0x4,
	contents_grate = 0x8,
	contents_slime = 0x10,
	contents_water = 0x20,
	contents_mist = 0x40,
	contents_opaque = 0x80,
	last_visible_contents = 0x80,
	all_visible_contents = ( last_visible_contents | ( last_visible_contents - 1 ) ),
	contents_testfogvolume = 0x100,
	contents_unused5 = 0x200,
	contents_unused6 = 0x4000,
	contents_team1 = 0x800,
	contents_team2 = 0x1000,
	contents_ignore_nodraw_opaque = 0x2000,
	contents_moveable = 0x4000,
	contents_areaportal = 0x8000,
	contents_playerclip = 0x10000,
	contents_monsterclip = 0x20000,
	contents_current_0 = 0x40000,
	contents_current_90 = 0x80000,
	contents_current_180 = 0x100000,
	contents_current_270 = 0x200000,
	contents_current_up = 0x400000,
	contents_current_down = 0x800000,
	contents_origin = 0x1000000,
	contents_monster = 0x2000000,
	contents_debris = 0x4000000,
	contents_detail = 0x8000000,
	contents_translucent = 0x10000000,
	contents_ladder = 0x20000000,
	contents_hitbox = 0x40000000,
};

enum trace_masks_t : unsigned int
{
	mask_all = ( 0xffffffff ),
	mask_solid = ( contents_solid | contents_moveable | contents_window | contents_monster | contents_grate ),
	mask_playersolid = ( contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate ),
	mask_npcsolid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate ),
	mask_water = ( contents_water | contents_moveable | contents_slime ),
	mask_opaque = ( contents_solid | contents_moveable | contents_opaque),
	mask_opaque_and_npcs = ( mask_opaque | contents_monster),
	mask_visible = ( mask_opaque | contents_ignore_nodraw_opaque ),
	mask_visible_and_npcs = ( mask_opaque_and_npcs | contents_ignore_nodraw_opaque ),
	mask_shot = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox ),
	mask_shot_hull = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate ),
	mask_shot_portal = ( contents_solid | contents_moveable | contents_window ),
	mask_solid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_grate ),
	mask_playersolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate ),
	mask_npcsolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate ),
	mask_npcworldstatic = ( contents_solid | contents_window | contents_monsterclip | contents_grate ),
};

struct player_t;

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

	ray_t( math::vec3_t src, math::vec3_t dst )
	{
		initialize( src, dst );
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

	trace_filter_entity( void* entity )
	{
		skip = entity;
	}
};

struct ITrace
{
	/*
	virtual int get_point_contents( const math::vec3_t& pos, int mask = MASK_ALL, IHandleEntity** ent = nullptr ) = 0;
	virtual int get_point_contents_world( const math::vec3_t& pos, int mask = MASK_ALL ) = 0;
	virtual int get_point_contents_collideable( void* collide, const math::vec3_t& pos ) = 0;
	virtual void clip_ray_to_entity( const ray_t& ray, unsigned int mask, player_t* ent, trace_t* trace ) = 0;
	virtual void clip_ray_to_collideable( const ray_t& ray, unsigned int mask, void* collide, trace_t* trace ) = 0;
	virtual void TraceRay( const ray_t& ray, unsigned int mask, trace_filter* filter, trace_t* trace ) = 0;
	*/

	VFUNC( 5, TraceRay( const ray_t& ray, unsigned int mask, trace_filter* filter, trace_t* trace ), void( __thiscall* )( void*, const ray_t&, unsigned int, trace_filter*, trace_t* ) )( ray, mask, filter, trace );
};