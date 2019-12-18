#pragma once

struct AutowallData
{
	math::vec3_t dst;
	math::vec3_t dir;
	
	Trace enter;
	Trace exit;

	float damage;
	float penetration;

	SurfaceData* surface;
};

extern AutowallData autowall_data;

namespace autowall
{
	void handle_bullet_penetration( AutowallData& data, math::vec3_t& dst );
}