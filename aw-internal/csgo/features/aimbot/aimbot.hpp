#pragma once

namespace aimbot
{
	struct AimbotData
	{
		AimbotData(player_t* pl, int i, int dmg, math::vec3_t p, float dist) :
			player(pl),
			index(i),
			damage(dmg),
			best_point(p),
			distance(dist)
		{};

		math::vec3_t best_point;
		player_t* player;
		int index;
		int damage;
		float distance;
	};

	int shots_fired;

	player_t* last_target = nullptr;
	std::vector<AimbotData> players;

	void choose_angles();
	void select_target();
	void restore_players();
	bool get_points_from_hitbox();
	void quickstop();
	bool is_valid();
	bool hitchance();
	void work();
}