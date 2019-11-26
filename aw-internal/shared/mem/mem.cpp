#include "../shared.hpp"

namespace mem
{
	address_t find_ida_sig( const char* mod, const char* sig )
	{
		static auto pattern_to_byte = []( const char* pattern )
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast< char* >( pattern );
			auto end = const_cast< char* >( pattern ) + strlen( pattern );

			for ( auto current = start; current < end; ++current )
			{
				if ( *current == '?' )
				{
					++current;

					if ( *current == '?' )
						++current;

					bytes.push_back( -1 );
				}
				else
				{
					bytes.push_back( strtoul( current, &current, 16 ) );
				}
			}
			return bytes;
		};

		const auto module_handle = GetModuleHandleA( mod );
		if ( !module_handle )
			return {};

		MODULEINFO module_info;
		GetModuleInformation( GetCurrentProcess(), reinterpret_cast< HMODULE >( module_handle ), &module_info, sizeof( MODULEINFO ) );

		const auto image_size = module_info.SizeOfImage;

		if ( !image_size )
			return {};

		auto pattern_bytes = pattern_to_byte( sig );
		const auto image_bytes = reinterpret_cast< byte* >( module_handle );

		const auto signature_size = pattern_bytes.size();
		const auto signature_bytes = pattern_bytes.data();

		for ( auto i = 0ul; i < image_size - signature_size; ++i )
		{
			auto byte_sequence_found = true;

			for ( auto j = 0ul; j < signature_size; ++j )
			{
				if ( image_bytes[ i + j ] != signature_bytes[ j ] && signature_bytes[ j ] != -1 ) {
					byte_sequence_found = false;
					break;
				}
			}

			if ( byte_sequence_found )
				return address_t( uintptr_t( &image_bytes[ i ] ) );
		}

		return {};
	}

	address_t find_ida_sig( const char* mod, const std::vector<const char*>& sigs )
	{
		address_t dummy{};
		auto sigs_tried = uint32_t();

		for ( auto sig : sigs )
			if ( dummy = find_ida_sig( mod, sig ) )
				break;
			else
				return dummy;
	}

	address_t find_fizz_sig( const char* mod, const char* sig )
	{
		static auto pattern_to_byte = []( const char* pattern )
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast< char* >( pattern );
			auto end = const_cast< char* >( pattern ) + strlen( pattern );

			for ( auto current = start; current < end; ++current )
			{
				if ( *current == '..' )
				{
					++current;

					if ( *current == '..' )
						++current;

					bytes.push_back( -1 );
				}
				else
				{
					bytes.push_back( strtoul( current, &current, 16 ) );
				}
			}
			return bytes;
		};

		const auto module_handle = GetModuleHandleA( mod );
		if ( !module_handle )
			return {};

		MODULEINFO module_info;
		GetModuleInformation( GetCurrentProcess(), reinterpret_cast< HMODULE >( module_handle ), &module_info, sizeof( MODULEINFO ) );

		const auto image_size = module_info.SizeOfImage;

		if ( !image_size )
			return {};

		auto pattern_bytes = pattern_to_byte( sig );
		const auto image_bytes = reinterpret_cast< byte* >( module_handle );

		const auto signature_size = pattern_bytes.size();
		const auto signature_bytes = pattern_bytes.data();

		for ( auto i = 0ul; i < image_size - signature_size; ++i )
		{
			auto byte_sequence_found = true;

			for ( auto j = 0ul; j < signature_size; ++j )
			{
				if ( image_bytes[ i + j ] != signature_bytes[ j ] && signature_bytes[ j ] != -1 )
				{
					byte_sequence_found = false;
					break;
				}
			}

			if ( byte_sequence_found )
				return address_t( uintptr_t( &image_bytes[ i ] ) );
		}

		return {};
	}

	address_t find_fizz_sig( const char* mod, const std::vector<const char*>& sigs )
	{
		address_t dummy{};
		auto sigs_tried = uint32_t();

		for ( auto sig : sigs )
		{
			if ( dummy = find_fizz_sig( mod, sig ) )
				break;
		}

		return dummy;
	}

	address_t get_vfunc( address_t table, uint16_t index )
	{
		return table.get<uintptr_t*>()[ index ];
	}

	uint32_t get_vtable_length( address_t table )
	{
		auto length = uint32_t{};

		for ( length = 0; table.cast<uintptr_t*>()[ length ]; length++ )
		{
			if ( IS_INTRESOURCE( table.cast<uintptr_t*>()[ length ] ) )
				break;
		}

		return length;
	}

	std::vector<std::wstring>& get_loaded_modules()
	{
		static std::vector<std::wstring> m_modules{};

		if ( m_modules.empty() )
		{
			auto peb = reinterpret_cast< PEB* >( reinterpret_cast< TEB* >( __readfsdword( 0x18 ) )->ProcessEnvironmentBlock );
			if ( !peb || !peb->Ldr->InMemoryOrderModuleList.Flink )
				return m_modules;

			auto module_list = &peb->Ldr->InMemoryOrderModuleList;
			for ( auto i = module_list->Flink; i != module_list; i = i->Flink )
			{
				auto entry = CONTAINING_RECORD( i, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
				if ( !entry )
					continue;

				auto path = std::wstring( entry->FullDllName.Buffer );
				auto module_name_begin = path.find_last_of( '\\' );

				if ( module_name_begin != std::wstring::npos )
					m_modules.push_back( path.substr( module_name_begin + 1 ) );
			}
		}
		return m_modules;
	}
}