#include "../csgo.hpp"

bool player_t::get_bbox( math::vec4_t& box )
{
	math::matrix3x4_t& tran_frame = get_coord_frame();

	const math::vec3_t min = GetCollideable()->OBBMins();
	const math::vec3_t max = GetCollideable()->OBBMaxs();

	math::vec3_t screen_boxes[ 8 ];

	math::vec3_t points[] =
	{
		{ min.x, min.y, min.z },
		{ min.x, max.y, min.z },
		{ max.x, max.y, min.z },
		{ max.x, min.y, min.z },
		{ max.x, max.y, max.z },
		{ min.x, max.y, max.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z }
	};

	for ( int i = 0; i <= 7; i++ )
	{
		math::vec3_t out_vec; math::vector_transform( points[ i ], tran_frame, out_vec );
		if ( !static_cast< bool >( ctx::csgo.debugoverlay->WorldToScreen( out_vec, screen_boxes[ i ] ) != 1 ) )
			return false;
	}

	math::vec3_t box_array[] = {
		screen_boxes[ 3 ],
		screen_boxes[ 5 ],
		screen_boxes[ 0 ],
		screen_boxes[ 4 ],
		screen_boxes[ 2 ],
		screen_boxes[ 1 ],
		screen_boxes[ 6 ],
		screen_boxes[ 7 ]
	};

	float left = screen_boxes[ 3 ].x, bottom = screen_boxes[ 3 ].y, right = screen_boxes[ 3 ].x, top = screen_boxes[ 3 ].y;

	for ( int i = 0; i <= 7; i++ )
	{
		if ( left > box_array[ i ].x )
			left = box_array[ i ].x;

		if ( bottom < box_array[ i ].y )
			bottom = box_array[ i ].y;

		if ( right < box_array[ i ].x )
			right = box_array[ i ].x;

		if ( top > box_array[ i ].y )
			top = box_array[ i ].y;
	}

	box.x = left;
	box.y = top;
	box.w = right - left;
	box.z = bottom - top;

	return true;
}

player_info_t player_t::get_player_info()
{
	if ( !this )
		return {};

	player_info_t info;
	ctx::csgo.engine->GetPlayerInfo( this->Index(), &info );
	
	return info;
}

math::vec3_t player_t::get_eye_pos()
{
	auto pos = math::vec3_t{};
	weapon_shootpos( &pos );

	return pos;
}

math::vec3_t player_t::get_hitbox_pos( int hitbox )
{
	math::matrix3x4_t bone_matrix[ 128 ];

	if ( !SetupBones( bone_matrix, 128, 256, 0.0f ) )
			return math::vec3_t{};

	auto studio_model = ctx::csgo.modelinfo->GetStudioModel( GetModel() );

	if( studio_model )
	{
		auto hitbox_set = studio_model->hitbox_set( 0 )->hitbox( hitbox );

		if ( hitbox_set )
		{
			auto min = math::vec3_t{}, max = math::vec3_t{};

			math::vector_transform( hitbox_set->mins, bone_matrix[ hitbox_set->bone ], min );
			math::vector_transform( hitbox_set->maxs, bone_matrix[ hitbox_set->bone ], max );

			return math::vec3_t{ ( min.x + max.x ) * 0.5f, ( min.y + max.y ) * 0.5f, ( min.z + max.z ) * 0.5f };
		}
	}
}

bool player_t::is_alive()
{
	if ( !this )
		return false;

	return get_lifestate() == ALIVE;
}

bool player_t::is_enemy()
{
	if ( !this || !ctx::client.local )
		return false;

	return ctx::client.local->get_team() != this->get_team();
}
