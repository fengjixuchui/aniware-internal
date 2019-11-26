#pragma once

struct hook_info_t
{
	hook_info_t() = default;
	hook_info_t( int index, void* replace ) : m_index( index ), m_replace( replace ) {}

	int m_index;
	void* m_replace;
};

struct hook_t
{
	hook_t() = default;
	hook_t( void* table ) : m_table( table ), m_hook( hook::hook_t( uintptr_t( table ) ) )
	{
		m_hook.init();
	}

	void* m_table;
	hook::hook_t m_hook;
	std::vector<hook_info_t> m_hook_info;
};

struct hook_handler_t;

namespace hooks
{
	std::unordered_map<uint32_t, hook_t>& get_map();
	hook_handler_t& get();

	void init();
	void undo();
}

template< typename func_sig >
inline func_sig create_hook_impl( const uint32_t token, void* table, const uint32_t index, void* func )
{
	auto& map = hooks::get_map();

	if ( map.find( token ) == map.end() )
		map.insert( std::make_pair( token, hook_t( table ) ) );

	auto & map_entry = map[ token ];
	auto & current_info = map_entry.m_hook_info.emplace_back( index, func );

	try
	{
		map_entry.m_hook.hook( current_info.m_index, current_info.m_replace );

		return map_entry.m_hook.get_original<func_sig>( current_info.m_index );
	}
	catch ( const std::out_of_range & err ){}

	return reinterpret_cast< func_sig >( nullptr );
}

inline bool create_event_callback_impl( std::string_view event_name, const std::function<void( IGameEvent* e )>& callback )
{
	event_handler::add( event_name, callback );

	return true;
}

#define REGISTERS uintptr_t ecx, uintptr_t edx

#define CREATE_HOOK( hooker, index, func ) \
using type_fn_ ## func = decltype( &func ); \
type_fn_ ## func orig_ ## func = create_hook_impl< type_fn_ ## func >( HASH( #hooker ), hooker, index, reinterpret_cast<void*>(func) ); \

#define CREATE_HOOK_OVERRIDE( hooker, index, func, type ) \
using type_fn = type; \
type_fn_ ## func orig_ ## func = create_hook_impl< type_fn_ ## func >( HASH( #hooker ), hooker, index, reinterpret_cast<void*>(func) ); \

#define CREATE_EVENT_CALLBACK( func ) bool m_event_ ## func = create_event_callback_impl( #func, func );

struct hook_handler_t
{
	hook_handler_t() = default;
	~hook_handler_t() = default;

	static void __fastcall paint( REGISTERS, int mode );
	static bool __fastcall create_move( REGISTERS, float flInputSampleTime, CUserCmd* cmd );
	static void __fastcall reload_fonts( REGISTERS );

	static void player_hurt( IGameEvent* e );

	CREATE_HOOK( ctx::csgo.enginevgui, 14, paint );
	CREATE_HOOK( ctx::csgo.clientmode, 24, create_move );
	CREATE_HOOK( ctx::csgo.scheme_manager, 4, reload_fonts );

	CREATE_EVENT_CALLBACK( player_hurt );
};