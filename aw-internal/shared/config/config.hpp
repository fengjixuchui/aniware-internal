#pragma once

#include "../hash/hash.hpp"

namespace config
{
	struct item_t
	{
		item_t( ) = default;
		item_t( const hash_t name, const hash_t type, const std::any var )
			: m_name( name ), m_type( type ), m_var( var )
		{};

		~item_t( ) = default;

		template< typename t >
		t& get( )
		{
			return *reinterpret_cast< t* >( std::any_cast< t >( &m_var ) );
		}

		template< typename t >
		void set( t val )
		{
			m_var.emplace<t>( val );
		}

		hash_t m_name;
		hash_t m_type;
		std::any m_var;
	};

	std::vector<item_t>& get_items( );

	template< typename t >
	uint32_t add_item( const hash_t name, const hash_t type, const t def )
	{
		get_items( ).push_back( item_t( name, type, std::make_any<t>( def ) ) );

		return get_items( ).size( ) - 1u;
	}

	template< typename t >
	t& get( const uint32_t index )
	{
		return get_items( ).at( index ).get<t>( );
	}

	item_t& get_item( const uint32_t index );

	int does_item_exist( const hash_t name );

	void save( );
	void load( );

	void set_config_file( const std::string_view config );

	std::string_view get_config_file( );

	void set_config_directory( const std::string_view name );

	std::string_view get_config_directory( );
}

#define ADD_CFG_ITEM( type, name, def ) const uint32_t name = config::add_item<type>( CT_HASH( #name ), CT_HASH( #type ), def );
#define ADD_CFG_ITEM_VEC( type, datatype, size, name, def ) const uint32_t name = config::add_item<std::vector<datatype>>( CT_HASH( #name ), CT_HASH( #type ), misc::create_filled_vector<datatype, size>( def ) );