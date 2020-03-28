#include "csgo.hpp"

namespace ctx
{
	csgo_t csgo = csgo_t{};
	client_t client = client_t{};
	mem_t mem = mem_t{};
	cfg_t cfg = cfg_t{};
}

int __stdcall detach()
{
	event_handler::undo();

	input::undo();
	hooks::undo();

	return TRUE;
}

unsigned long __stdcall entry( void* instance )
{
	try
	{
		input::init( L"Valve001" );

		netvars::init();
		classids.init();
		render::init();
		menu::init();
		hooks::init();

		config::set_config_directory( "aniware_cfg" );

		while ( !input::get_key_info( VK_END ).m_state )
			std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
	}
	catch ( const std::runtime_error& err )
	{
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::this_thread::sleep_for( std::chrono::milliseconds( 400 ) );

	detach();
	FreeLibraryAndExitThread( static_cast< HMODULE >( instance ), EXIT_SUCCESS );
}

int __stdcall DllMain( void* instance, unsigned long call_reason, void* reserved )
{
	if ( call_reason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( static_cast< HMODULE >( instance ) );
		if ( auto handle = CreateThread( nullptr, NULL, entry, instance, NULL, nullptr ) )
		{
			CloseHandle( handle );
		}
	}
	else if ( call_reason == DLL_PROCESS_DETACH && !reserved )
	{
		return detach();
	}

	return TRUE;
}