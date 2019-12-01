#pragma once

enum hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_LOWER_CHEST,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

struct studio_bone_t
{
	int name_index;
	int parent;
	int bone_controller[ 6 ];

	math::vec3_t pos;
	math::vec4_t quaternion;
	math::vec3_t rotation;

	math::vec3_t pos_scale;
	math::vec3_t rot_scale;

	math::matrix3x4_t pose_to_bone;
	math::vec4_t quat_alignement;
	int flags;
	int proc_type;
	int proc_index;
	int physics_bone;

	int contents;
	int surf_prop_lookup;
	int unused[ 7 ];
};

struct studio_box_t
{
	int bone;
	int group;
	math::vec3_t mins;
	math::vec3_t maxs;
	int name_index;
	int pad01[ 3 ];
	float radius;
	int pad02[ 4 ];
};

struct studio_hitbox_set_t
{
	int name_index;
	int hitbox_count;
	int hitbox_index;

	studio_box_t* hitbox( int index )
	{
		return reinterpret_cast<studio_box_t* >( reinterpret_cast< unsigned char *>( this ) + hitbox_index)  + index;
	}
};

struct studio_hdr_t 
{
	int id;
	int version;
	long checksum;
	char name_char_array[ 64 ];
	int length;

	math::vec3_t eye_pos;
	math::vec3_t illium_pos;
	math::vec3_t hull_mins;
	math::vec3_t hull_maxs;
	math::vec3_t mins;
	math::vec3_t maxs;

	int flags;
	int bones_count;
	int bone_index;
	int bone_controllers_count;
	int bone_controller_index;
	int hitbox_sets_count;
	int hitbox_set_index;
	int local_anim_count;
	int local_anim_index;
	int local_seq_count;
	int local_seq_index;
	int activity_list_version;
	int events_indexed;
	int textures_count;
	int texture_index;

	studio_hitbox_set_t* hitbox_set( int index )
	{
		if ( index > hitbox_sets_count )
			return nullptr;

		return reinterpret_cast< studio_hitbox_set_t* >(reinterpret_cast<unsigned char*>(this) + hitbox_set_index ) + index;
	}

	studio_bone_t* bone( int index)
	{
		if ( index > bones_count )
			return nullptr;

		return reinterpret_cast< studio_bone_t* >( reinterpret_cast< unsigned char *>( this ) + bone_index )  + index;
	}
};