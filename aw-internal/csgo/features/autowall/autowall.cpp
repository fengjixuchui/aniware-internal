#include "../../csgo.hpp"

namespace autowall
{
	void scale_damage( Trace_t& trace, WeaponInfo_t* data, float& damage )
	{
		player_t* pl = reinterpret_cast< player_t* >( trace.entity );
		
		if ( !pl || !pl->is_alive() )
			return;

		const auto heavy_armor = pl->has_heavy_armor();
		const auto armor_value = pl->get_armor();

		const auto is_armored = [&]( Trace_t& tr ) -> bool
		{
			player_t* pl = reinterpret_cast< player_t* >( tr.entity );

			switch ( static_cast< Hitgroups >( tr.hitgroup ) )
			{
			case HITGROUP_HEAD:
				return pl->has_helmet();
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				return true;
			default:
				return false;
			}
		};

		switch ( static_cast< Hitgroups >( trace.hitgroup ) )
		{
		case HITGROUP_HEAD:
			damage *= heavy_armor ? 2.f : 4.f;
			break;
		case HITGROUP_STOMACH:
			damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			damage *= 0.75f;
			break;
		default:
			break;
		}

		if ( armor_value > 0 && is_armored( trace ) )
		{
			float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = data->armor_ratio / 2.f;

			if ( heavy_armor )
			{
				armor_bonus_ratio = 0.33f;
				armor_ratio *= 0.5f;
				bonus_value = 0.33f;
			}

			auto new_damage = damage * armor_ratio;

			if ( heavy_armor )
				new_damage *= 0.85f;

			if ( ( ( damage - ( damage * armor_ratio ) ) * ( bonus_value * armor_bonus_ratio ) ) > armor_value )
				new_damage = damage - ( armor_value / armor_bonus_ratio );

			damage = new_damage;
		}
	}

	void trace_line(const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, void* skip, Trace_t* trace)
	{
		if ( !src.valid() || !dst.valid() )
			return;

		Ray_t ray;
		TraceFilter filter;

		ray.initialize( src, dst );
		filter.skip = skip;
		
		ctx::csgo.enginetrace->TraceRay( ray, mask, &filter, trace );
	}

	void clip_trace_to_players( const math::vec3_t& src, math::vec3_t& dst, unsigned int mask, TraceFilter* filter, Trace_t* trace )
	{
		static const auto clip_trace_to_players_fn = mem::find_ida_sig( "client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC D8 ?? ?? ?? 0F 57 C9" ).cast< int >();
		
		if ( !clip_trace_to_players_fn )
			return;

		_asm {
			mov		eax, filter
			lea		ecx, trace
			push	ecx
			push	eax
			push	mask
			lea		edx, dst
			lea		ecx, src
			call	clip_trace_to_players_fn
			add		esp, 0xC
		}
	}

	bool is_breakable( entity_t* ent )
	{
		if ( !ent || !ent->EntIndex() )
			return false;

		const int take_damage = 0x280;
		const int old_damage = ent->read< int >( take_damage );

		const ClientClass* client_class = ent->GetClientClass();

		if ( !client_class )
			return false;

		const std::string network_name = client_class->m_pNetworkName;
		try {

			if (network_name.at( 1 ) == 'B' && network_name.at( 9 ) == 'e' && network_name.at( 10 ) == 'S' && network_name.at( 16 ) == 'e')
				ent->write< int >( take_damage, DAMAGE_YES );

			else if (network_name.at( 1 ) != 'B' && network_name.at( 5 ) != 'D')
				ent->write< int >( take_damage, DAMAGE_YES );

			else if (network_name.at( 1 ) != 'F' && network_name.at( 4 ) != 'c' && network_name.at( 5 ) != 'B' && network_name.at( 9 ) != 'h')
				ent->write< int >( take_damage, DAMAGE_YES );

		}
		catch ( std::out_of_range & ex ) {}

		using is_breakable_entity_t = bool( __thiscall* )( entity_t* );
		static const auto is_breakable_entity_fn = mem::find_ida_sig( "client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68" ).cast< is_breakable_entity_t >();

		ent->write< int >( take_damage, old_damage );

		return is_breakable_entity_fn( ent );
	}

	bool trace_to_exit( math::vec3_t& end, const math::vec3_t& start, const math::vec3_t& dir, Trace_t* enter_trace, Trace_t* exit_trace)
	{
		float dist = 0.f;

		while ( dist <= 90.f ) {
			dist += 4.f;
			
			end = start + dir * dist;

			const int point_contents = ctx::csgo.enginetrace->GetPointContents( end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );

			if ( point_contents & MASK_SHOT_HULL && !( point_contents & CONTENTS_HITBOX ) )
				continue;

			Ray_t ray1;
			ray1.initialize( end, end - dir * 4.f );

			ctx::csgo.enginetrace->TraceRay( ray1, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, exit_trace );

			if ( exit_trace->start_solid && exit_trace->surface.flags & SURF_HITBOX )
			{
				TraceFilter filter;
				filter.skip = exit_trace->entity;

				Ray_t ray2;
				ray2.initialize( end, start );

				ctx::csgo.enginetrace->TraceRay( ray2, MASK_SHOT_HULL, &filter, exit_trace );

				if ( ( exit_trace->fraction < 1.f || exit_trace->all_solid ) && !exit_trace->start_solid )
				{
					end = exit_trace->end;
					return true;
				}

				continue;
			}

			if ( !exit_trace->did_hit() || exit_trace->start_solid )
			{
				if (enter_trace->entity && (enter_trace->entity != nullptr && enter_trace->entity != ctx::client.local ) )
				{
					*exit_trace = *enter_trace;
					exit_trace->end = start + dir;
					return true;
				}

				continue;
			}

			if ( !exit_trace->did_hit() || exit_trace->start_solid )
			{
				if ( enter_trace->entity != nullptr && !enter_trace->entity->EntIndex() == 0 && is_breakable( static_cast< player_t* >( enter_trace->entity ) ) )
				{
					*exit_trace = *enter_trace;
					exit_trace->end = start + dir;
					return true;
				}

				continue;
			}

			if ( exit_trace->surface.flags >> 7 & SURF_LIGHT && !( enter_trace->surface.flags >> 7 & SURF_LIGHT ) )
			{
				continue;
			}				

			if ( exit_trace->plane.normal.dot( dir ) <= 1.f )
			{
				end = end - dir * ( exit_trace->fraction * 4.f );
				return true;
			}
		}

		return false;
	}
}