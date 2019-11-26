#pragma once

#include "../mem/mem.hpp"
#include "../mem_protect/mem_protect.hpp"

namespace hook
{
	struct hook_t
	{
		hook_t() = default;
		hook_t( uintptr_t ptr ) : m_vtable( reinterpret_cast< uintptr_t** >( ptr ) ), m_table_length( 0 ), m_orig( nullptr ), m_replace( nullptr ) {};
		hook_t( void* ptr ) : m_vtable( reinterpret_cast< uintptr_t** >( ptr ) ), m_table_length( 0 ), m_orig( nullptr ), m_replace( nullptr ) {};
		hook_t( address_t ptr ) : m_vtable( ptr.cast<uintptr_t**>() ), m_table_length( 0 ), m_orig( nullptr ), m_replace( nullptr ) {};

		bool init()
		{
			if ( !m_vtable )
				return false;

			INIT_MEM_PROTECT_RW( m_vtable, sizeof( uintptr_t ) );
			m_orig = *m_vtable;

			m_table_length = mem::get_vtable_length( m_orig );

			if ( !m_table_length )
				return false;

			m_replace = std::make_unique<uintptr_t[]>( m_table_length + 1 );

			std::memset( m_replace.get(), NULL, m_table_length * sizeof( uintptr_t ) + sizeof( uintptr_t ) );
			std::memcpy( &m_replace[ 1 ], m_orig, m_table_length * sizeof( uintptr_t ) );
			std::memcpy( m_replace.get(), &m_orig[ -1 ], sizeof( uintptr_t ) );

			*m_vtable = &m_replace[ 1 ];
			return true;
		}

		template< typename t >
		void hook( const uint16_t index, t replace_function )
		{
			if ( index < 0 || index > m_table_length )
				throw std::out_of_range( fmt::format( "hook_t::hook - Trying to hook at faulty index {} while max index is {}", index, m_table_length ) );

			m_replace[ index + 1 ] = reinterpret_cast< uintptr_t >( replace_function );
		}

		template< typename t >
		t get_original( const uint16_t index )
		{
			if ( index < 0 || index > m_table_length )
				throw std::out_of_range( fmt::format( "hook_t::get_original - Trying to get original function of hook at faulty index {} while max index is {}", index, m_table_length ) );

			return reinterpret_cast< t >( m_orig[ index ] );
		}

		void unhook( const uint16_t index )
		{
			if ( index < 0 || index > m_table_length )
				throw std::out_of_range( fmt::format( "hook_t::unhook - Trying to unhook at faulty index {} while max index is {}", index, m_table_length ) );

			m_replace[ index + 1 ] = m_orig[ index ];
		}

		void unhook()
		{
			if ( !m_orig )
				return;

			INIT_MEM_PROTECT_RW( m_vtable, sizeof( uintptr_t ) );

			*m_vtable = m_orig;
			m_orig = nullptr;
		}

		uintptr_t** m_vtable;
		uint16_t m_table_length;
		uintptr_t* m_orig;

		std::unique_ptr<uintptr_t[]> m_replace;
	};
}