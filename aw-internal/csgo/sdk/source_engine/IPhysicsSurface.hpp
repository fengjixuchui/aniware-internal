#pragma once

struct SurfaceData
{
	char pad0x0[ 0x50 ];
	float maxspeedfactor;
	float jumpfactor;
	float penetrationmodifier;
	float damagemodifier;
	short material;
	bool climbable;
};

struct IPhysicsSurface
{
	VFUNC( 5, GetSurfaceData( int index ), SurfaceData*( __thiscall* )( void*, int ) )( index );
};