#include "../../csgo.hpp"

float __stdcall hook_handler_t::viewmodel_fov()
{
	static float view_fov = ctx::csgo.cvar->FindVar("viewmodel_fov")->GetFloat();

	if ( ctx::client.local && ctx::client.local->is_alive() )
		return ctx::client.local->is_scoped() ? 0.f : view_fov + config::get<float>( ctx::cfg.view_fov );

	return hooks::get().orig_viewmodel_fov();
}
