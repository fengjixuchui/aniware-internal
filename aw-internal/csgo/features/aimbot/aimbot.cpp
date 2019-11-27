#include "../../csgo.hpp"

namespace aimbot
{
	void choose_angles()
	{

	}

	void select_target()
	{

	}

	void restore_players()
	{

	}

	bool get_points_from_hitbox()
	{
		return false;
	}

	void quickstop()
	{

	}

	bool is_valid()
	{
		return false;
	}

	bool hitchance( math::vec3_t& ang, player_t* pl )
	{
		math::vec3_t forward, right, up;

		const math::vec3_t eye_position = ctx::client.local->get_eye_pos();
		const math::vec3_t punch_angle = ctx::client.local->get_punch_angle();

		int traces_hit = 0;
		math::angle_vectors( math::angle_t{ ang + punch_angle * 2.f }, forward, right, up );

		auto weapon = entity_t::get<weapon_t>( ctx::client.local->get_weapon_handle() );
		if ( !weapon )
			return false;

		/*
		weapon->update_accuracy();
		float weapon_spread = weapon->spread();
		float weapon_cone = weapon->inaccuracy();

		const auto get_bullet_location = [&](int seed) {
			static auto random_seed = pe::get_export< void(*)(int) >(pe::get_module("vstdlib.dll"), "RandomSeed");
			random_seed(seed);

			float a = math::random_float(0.f, 1.f);
			float b = math::random_float(0.f, 2.f * math::pi);
			float c = math::random_float(0.f, 1.f);
			float d = math::random_float(0.f, 2.f * math::pi);

			const float generated_spread = a * weapon_spread;
			const float generated_cone = c * weapon_cone;

			const vec3_t spread = vec3_t(
				std::cos(b) * generated_spread + std::cos(d) * generated_cone,
				std::sin(b) * generated_spread + std::sin(d) * generated_cone,
				0.f
			);

			return vec3_t(forward + right * -spread.x + up * -spread.y).normalized();
		};

		for (int i = 1; i <= 256; i++) {
			vec3_t spread_angle;
			vec3_t bullet_end;

			math::vector_angle(get_bullet_location(i), spread_angle);
			math::angle_to_vector(angle - (spread_angle - angle), bullet_end);

			trace_t trace;
			ray_t ray;
			ray.init(eye_position, eye_position + bullet_end * weapon->get_weapon_info()->range);

			g_csgo.m_engine_trace->clip_ray_to_entity(ray, MASK_SHOT, ent, &trace);

			if (trace.m_hit_entity == ent)
				++traces_hit;

			if (traces_hit >= static_cast<int>(g_vars.rage.hitchance * 2.56f))
				return true;
		}
		*/
		return false;
	}

	void work()
	{
		if ( !config::get<bool>( ctx::cfg.aim_enable ) )
			return;

		select_target();
		choose_angles();
		restore_players();

		if ( !players.empty() )
			players.clear();
	}
}