#pragma once

#include "../../context/mem.hpp"

struct CEconItemDefinition
{
	char pad_0x0000[ 0x8 ]; 	int32_t m_iItemDefinitionIndex; 	char pad_0x000C[ 0x40 ]; 	char* m_szHudName; 	char pad_0x0050[ 0x4 ]; 	char* m_szWeaponType; 	char pad_0x0058[ 0x4 ]; 	char* m_szWeaponDescription; 	char pad_0x0060[ 0x34 ]; 	char* m_szViewModel; 	char pad_0x0098[ 0x4 ]; 	char* m_szWorldModel; 	char* m_szWorldModelDropped; 	char pad_0x00A4[ 0x110 ]; 	char* m_szWeaponName1; 	char pad_0x01B8[ 0x8 ]; 	char* m_szWeaponName; 	char pad_0x01C4[ 0x27C ]; }; 
struct CCStrike15ItemSchema
{
	VFUNC( 3, GetItemDefinitionCount(), int( __thiscall* )( void* ) )( );

	CUSTOM_VFUNC( GetItemDefinitionByName( const char* name ), CEconItemDefinition* ( __thiscall* )( void*, const char* ), ctx::mem.CItemSystem.GetItemDefinitionByName )( name );

	uintptr_t GetItemDefinitionMap()
	{
		return address_t( uintptr_t( this ) + ctx::mem.CItemSystem.m_Map ).get( 1 );
	}

		CEconItemDefinition* GetItemDefinitionByMapIndex( int i )
	{
		if ( i < 0 || i >= GetItemDefinitionCount() )
			return nullptr;

				return *reinterpret_cast< CEconItemDefinition * * >( GetItemDefinitionMap() + 0x4 * ( i * 0x3 ) + sizeof( void* ) );
	}
};

struct CCStrike15ItemSystem
{
	virtual CCStrike15ItemSchema* GetItemSchemaInterface() = 0;
};