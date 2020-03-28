#include "../../csgo.hpp"

namespace interfaces
{
	using InstantiateInterfaceFn = void* ( __cdecl* )( );
	struct InterfaceReg
	{
		InstantiateInterfaceFn	m_CreateFn;
		const char* m_pName;

		InterfaceReg* m_pNext;
		static InterfaceReg* s_pInterfaceRegs;
	};

	std::unordered_map< uint32_t, std::vector< std::pair< const char*, address_t >>>& get_interface_map( )
	{
		static std::unordered_map< uint32_t, std::vector< std::pair< const char*, address_t >>> interfaces{};
		return interfaces;
	}

	bool build_interface_list( )
	{
		auto& interface_map = get_interface_map( );
		if ( interface_map.empty( ) )
		{
			for ( auto mod : mem::get_loaded_modules( ) )
			{
				auto module_address = GetModuleHandleW( mod.c_str( ) );
				if ( !module_address )
					continue;

				auto create_interface_fn = address_t( uintptr_t( GetProcAddress( module_address, "CreateInterface" ) ) );
				if ( !create_interface_fn )
					continue;
																				
				auto list = create_interface_fn.self_find_opcode( 0xE9 ).self_jmp( ).self_find_opcode( 0x35, 0x1 ).get< InterfaceReg* >( 2 ); 
				if ( !list )
					continue;

				auto& entry = interface_map[ HASH( misc::to_utf8( mod ).c_str( ) ) ];

				while ( list )
				{
					entry.push_back( std::make_pair( list->m_pName, uintptr_t( list->m_CreateFn( ) ) ) );
					list = list->m_pNext;
				}
			}
		}

		return !interface_map.empty( );
	}

	uintptr_t* get( const char* interface_name )
	{
		if ( !build_interface_list( ) )
			return nullptr;

		auto& interface_map = get_interface_map( );
		for ( auto& entry : interface_map )
		{
			if ( entry.second.empty( ) )
				return nullptr;

			for ( auto& cur : entry.second )
			{
				if ( std::string( cur.first ).find( interface_name ) != std::string::npos )
				{
					return cur.second.cast<uintptr_t*>( );
				}
			}
		}

				return nullptr;
	}

	uintptr_t* get( const char* mod_name, const char* interface_name )
	{
		if ( !build_interface_list( ) )
			return nullptr;

		auto& interface_map = get_interface_map( );

		auto& entry = interface_map[ HASH( mod_name ) ];
		if ( entry.empty( ) )
			return nullptr;

		for ( auto& cur : entry )
		{
			if ( std::string( cur.first ).find( interface_name ) != std::string::npos )
			{
				return cur.second.cast<uintptr_t*>( );
			}
		}

		return nullptr;
	}
}