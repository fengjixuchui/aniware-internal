#pragma once

namespace ctx
{
	struct cfg_t
	{
		ADD_CFG_ITEM( bool, aim_enable, false );
		ADD_CFG_ITEM( bool, aim_friendly, false );
		ADD_CFG_ITEM( bool, aim_silent, false );
		ADD_CFG_ITEM( bool, aim_shoot, false );
		ADD_CFG_ITEM( float, aim_hitchance, 0.f );

		ADD_CFG_ITEM( bool, lagcompensation, false );
		ADD_CFG_ITEM( float, lagcompensation_ms, 0.f );

		ADD_CFG_ITEM( bool, draw_fog, true );

		ADD_CFG_ITEM( float, view_fov, 0.f );
		ADD_CFG_ITEM( float, world_fov, 0.f );

		ADD_CFG_ITEM( bool, extrasensory_name, false );
		ADD_CFG_ITEM( int, extrasensory_box_type, 0 );
		ADD_CFG_ITEM( int, extrasensory_health_type, false) ;

		ADD_CFG_ITEM( bool, hitmarker_enable, false );
		ADD_CFG_ITEM( bool, hitmarker_tint, false );
		ADD_CFG_ITEM( int, hitmarker_sound, 0 );

		ADD_CFG_ITEM( bool, movement_bhop, false );
		ADD_CFG_ITEM( bool, movement_strafe_enable, false );
		ADD_CFG_ITEM( int, movement_strafe_type, 0 );
	};

	extern cfg_t cfg;
}