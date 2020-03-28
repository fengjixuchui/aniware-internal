#include "../../csgo.hpp"

AutowallData autowall_data;

namespace autowall
{
	void handle_bullet_penetration( AutowallData& data, math::vec3_t& dst )
	{
		const auto trace_to_exit = mem::find_ida_sig( "client_panorama.dll", "55 8B EC 83 EC 30 F3 0F 10 75" ).cast< bool( __thiscall* )( float, float, float, float, float, float, Trace& ) >( );

		if ( !trace_to_exit )
			return;

		__asm {
			mov ecx, autowall_data.dst
			mov edx, autowall_data.exit
		}

		const auto end = autowall_data.enter.end;
		const auto dir = autowall_data.dir;

		if ( !trace_to_exit( end.x, end.y, end.z, dir.x, dir.y, dir.z, autowall_data.exit ) )
			return;

		const auto enter = autowall_data.surface;
		const auto exit = ctx::csgo.physics_surface->GetSurfaceData( autowall_data.exit.surface.props );

		static float mod_damage = 0.16f;
		static float mod_penetration = ( enter->penetrationmodifier + exit->penetrationmodifier ) * 0.5f;

		if ( enter->material == 71 || enter->material == 89 )
		{
			mod_damage = 0.05f;
			mod_penetration = 3.0f;
		}
		else if ( autowall_data.enter.contents >> 3 & 1 || autowall_data.enter.surface.flags.m_flags >> 7 & 1 )
		{
			mod_penetration = 1.0f;
		}

		if ( enter->material == exit->material )
		{
			if ( exit->material == 85 || exit->material == 87 )
			{
				mod_penetration = 3.0f;
			}
			else if ( exit->material == 76 )
			{
				mod_penetration = 2.0f;
			}
		}

		autowall_data.damage -= ( 11.25f / autowall_data.penetration / mod_penetration + autowall_data.damage 
			* mod_damage + (autowall_data.exit.end - autowall_data.enter.end).length( ) / 24.0f / mod_penetration );

		dst = autowall_data.exit.end;
	}

	float get_damage( int hitgroup )
	{
		switch ( hitgroup )
		{
		case HITGROUP_HEAD:
			return 4.0f;
		case HITGROUP_STOMACH:
			return 1.25f;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return 0.75f;
		default:
			return 1.0f;
		}
	}

	bool get_armor( int hitgroup, bool helmet )
	{
		switch ( hitgroup )
		{
		case HITGROUP_HEAD:
			return helmet;
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	}
}