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

		const int old_damage = ent->read< int >( TAKE_DAMAGE );

		if ( const ClientClass* client_class = ent->GetClientClass() )
		{
			const std::string network_name = client_class->m_pNetworkName;

			if ( network_name[ 1 ] == 'B' && network_name[ 9 ] == 'e' && network_name[ 10 ] == 'S' && network_name[ 16 ] == 'e')
				ent->write< int >( TAKE_DAMAGE, DAMAGE_YES );

			else if ( network_name[ 1 ] != 'B' && network_name[ 5 ] != 'D' )
				ent->write< int >( TAKE_DAMAGE, DAMAGE_YES );

			else if ( network_name[ 1 ] != 'F' && network_name[ 4 ] != 'c' && network_name[ 5 ] != 'B' && network_name[ 9 ] != 'h' )
				ent->write< int >( TAKE_DAMAGE, DAMAGE_YES );
		}

		static const auto is_breakable_entity = mem::find_ida_sig( "client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68" ).cast< bool( __thiscall*)( void* ) >();

		ent->write< int >( TAKE_DAMAGE, old_damage );

		return is_breakable_entity( ent );
	}
}