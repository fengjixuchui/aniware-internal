#pragma once

namespace mem
{
	address_t find_ida_sig( const char* mod, const char* sig );
	address_t find_ida_sig( const char* mod, const std::vector< const char* >& sigs );
	address_t find_fizz_sig( const char* mod, const char* sig );
	address_t find_fizz_sig( const char* mod, const std::vector< const char* >& sigs );
	address_t get_vfunc( address_t table, uint16_t index );
	uint32_t get_vtable_length( address_t table );
	std::vector<std::wstring>& get_loaded_modules( );
}

#define NOINLINE __declspec( noinline )

#define MEMBER_FUNC_ARGS(...) ( this, __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return mem::get_vfunc( this, index ).cast< sig >( ) MEMBER_FUNC_ARGS
#define CUSTOM_VFUNC( func, sig, addr ) auto func { return addr.cast< sig >( ) MEMBER_FUNC_ARGS
#define OFFSET( type, func, offset ) type& func( ) { return *reinterpret_cast< type* >( reinterpret_cast< uintptr_t >( this ) + offset.cast( ) ); }