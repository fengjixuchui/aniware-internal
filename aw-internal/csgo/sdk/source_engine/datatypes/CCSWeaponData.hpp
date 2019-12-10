#pragma once

class WeaponInfo_t {
public:
	std::uint8_t pad_0x0000[ 0x4 ];
	char* weapon_name;
	std::uint8_t pad_0x0008[ 0xC ];
	std::uint32_t max_clip;
	std::uint8_t pad_0x0018[ 0x68 ];
	char* ammo_name;
	char* ammo_name2;
	char* hud_name;
	char* weapon_id;
	std::uint8_t pad_0x0090[ 0x3C ];
	std::uint32_t type;
	std::uint32_t price;
	std::uint32_t reward;
	std::uint8_t pad_0x00D8[ 0x14 ];
	std::uint8_t full_auto;
	std::uint8_t pad_0x00ED[ 0x3 ];
	std::uint32_t damage;
	float armor_ratio;
	std::uint32_t bullets;
	float penetration;
	std::uint8_t pad_0x0100[ 0x8 ];
	float range;
	float range_modifier;
	std::uint8_t pad_0x0110[ 0x20 ];
	float max_speed;
	float max_speed_alt;
	std::uint8_t pad_0x0138[ 0x108 ];
};