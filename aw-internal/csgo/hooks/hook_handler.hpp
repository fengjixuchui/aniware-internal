#pragma once

struct hook_info_t
{
	hook_info_t( ) = default;
	hook_info_t( int index, void* replace ) : m_index( index ), m_replace( replace ) {}

	int m_index;
	void* m_replace;
};

struct hook_t
{
	hook_t( ) = default;
	hook_t( void* table ) : m_table( table ), m_hook( hook::hook_t( uintptr_t( table ) ) )
	{
		m_hook.init( );
	}

	void* m_table;
	hook::hook_t m_hook;
	std::vector< hook_info_t > m_hook_info;
};

struct hook_handler_t;

namespace hooks
{
	std::unordered_map<uint32_t, hook_t>& get_map( );
	hook_handler_t& get( );

	void init( );
	void undo( );

	enum idx : int
	{
		SHOULD_DRAW_FOG = 17,
		OVERRIDE_VIEW = 18,
		CREATE_MOVE = 24,
		GET_VIEWMODEL_FOV = 35,
		DO_POST_SCREEN_SPACE_FX = 44,
		IS_HLTV = 93,
		LEVEL_INIT_POST_ENTITY = 6,
		LEVEL_SHUTDOWN = 7,
		FRAME_STAGE_NOTIFY = 37,
		LOCK_CURSOR = 67,
		DRAW_SET_COLOR = 15,
		RELOAD_FONT = 4,
		ENGINE_PAINT = 14,
		DRAW_MODEL_EXECUTE = 21,
		SCENE_END = 9,
		RENDER_SMOKE_OVERLAY = 41,
		GET_MATERIAL = 84,
		DO_EXTRA_BONE_PROC = 195,
		SETUP_BONES = 13
	};
}

template< typename func_sig >
inline func_sig create_hook_impl( const uint32_t token, void* table, const uint32_t index, void* func )
{
	auto& map = hooks::get_map( );

	if ( map.find( token ) == map.end( ) )
		map.insert( std::make_pair( token, hook_t( table ) ) );

	auto &map_entry = map[ token ];
	auto &current_info = map_entry.m_hook_info.emplace_back( index, func );

	try
	{
		map_entry.m_hook.hook( current_info.m_index, current_info.m_replace );

		return map_entry.m_hook.get_original< func_sig >( current_info.m_index );
	}
	catch ( const std::out_of_range &err ){}

	return reinterpret_cast< func_sig >( nullptr );
}

inline bool create_event_callback_impl( std::string_view event_name, const std::function< void( IGameEvent* e ) > &callback )
{
	event_handler::add( event_name, callback );
	return true;
}

#define REGISTERS uintptr_t ecx, uintptr_t edx

#define CREATE_HOOK( hooker, index, func ) \
using type_fn_ ## func = decltype( &func ); \
type_fn_ ## func orig_ ## func = create_hook_impl< type_fn_ ## func >( HASH( #hooker ), hooker, index, reinterpret_cast< void* >( func ) ); \

#define CREATE_HOOK_OVERRIDE( hooker, index, func, type ) \
using type_fn = type; \
type_fn_ ## func orig_ ## func = create_hook_impl< type_fn_ ## func >( HASH( #hooker ), hooker, index, reinterpret_cast< void* >( func ) ); \

#define CREATE_EVENT_CALLBACK( func ) bool m_event_ ## func = create_event_callback_impl( #func, func );

using namespace hooks;

struct hook_handler_t
{
	hook_handler_t( ) = default;
	~hook_handler_t( ) = default;

	static void __fastcall paint( REGISTERS, int mode );
	static bool __fastcall create_move( REGISTERS, float flInputSampleTime, CUserCmd* cmd );
	static void __fastcall frame_stage_notify( REGISTERS, client_frame_stage_t stage );
	static bool __fastcall draw_fog( REGISTERS );
	static float __stdcall viewmodel_fov( );
	static void __fastcall override_view( REGISTERS, CViewSetup* setup );
	static bool __fastcall do_screen_effects( REGISTERS, const CViewSetup* setup);
	static bool __fastcall is_hltv( REGISTERS );
	static void __fastcall reload_fonts( REGISTERS );

	static void player_hurt( IGameEvent* e );
	static void player_step( IGameEvent* e );

	CREATE_HOOK( ctx::csgo.enginevgui, idx::ENGINE_PAINT, paint );
	CREATE_HOOK( ctx::csgo.clientmode, idx::CREATE_MOVE, create_move );
	CREATE_HOOK( ctx::csgo.client, idx::FRAME_STAGE_NOTIFY, frame_stage_notify );
	CREATE_HOOK( ctx::csgo.clientmode, idx::SHOULD_DRAW_FOG, draw_fog );
	CREATE_HOOK( ctx::csgo.clientmode, idx::GET_VIEWMODEL_FOV, viewmodel_fov );
	CREATE_HOOK( ctx::csgo.clientmode, idx::OVERRIDE_VIEW, override_view );
	CREATE_HOOK( ctx::csgo.clientmode, idx::DO_POST_SCREEN_SPACE_FX, do_screen_effects );
	CREATE_HOOK( ctx::csgo.engine, idx::IS_HLTV, is_hltv );

	CREATE_HOOK( ctx::csgo.scheme_manager, idx::RELOAD_FONT, reload_fonts );

	CREATE_EVENT_CALLBACK( player_hurt );
	CREATE_EVENT_CALLBACK( player_step );
};
