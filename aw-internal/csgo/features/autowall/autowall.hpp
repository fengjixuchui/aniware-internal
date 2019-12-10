#pragma once

struct AutowallData
{
	SurfaceData* surface;
	
	Trace enter;
	Trace exit;

	float penetration;
	float damage;

	int hits;

	math::vec3_t dst;
	math::vec3_t dir;
};

extern AutowallData autowall_data;

namespace autowall
{

}