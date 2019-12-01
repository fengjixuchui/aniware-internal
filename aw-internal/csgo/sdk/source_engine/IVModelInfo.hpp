#pragma once

struct IVModelInfo
{
	VFUNC( 1, GetModel( const int index ), void * ( __thiscall* )( void*, const int ) )( index );
	VFUNC( 32, GetStudioModel( const void* model ), studio_hdr_t * ( __thiscall* )( void*, const void* ) )( model );
};