#pragma once

namespace misc
{
	template< typename t, size_t s >
	std::array<t, s> create_filled_array( const t& fill )
	{
		auto arr = std::array<t, s>{};
		arr.fill( fill );
		return arr;
	}

	template< typename t, size_t s >
	std::vector<t> create_filled_vector( const t& fill )
	{
		auto vec = std::vector<t>( s );
		std::fill( vec.begin( ), vec.begin( ) + s, fill );
		return vec;
	}

	std::string to_utf8( std::wstring_view wstr );
	std::wstring to_unicode( std::string_view str );
}