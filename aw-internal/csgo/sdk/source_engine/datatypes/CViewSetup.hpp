#pragma once

struct CViewSetup
{
	char pad_0x11[0x10];
	int x;
	int x_old;
	int y;
	int y_old;
	int width;
	int width_old;
	int height;
	int height_old;
	char pad_0x81[0x80];
	float fov;
	float fov_viewmodel;
	math::vec3_t origin;
	math::vec3_t angles;
	float z_near;
	float z_far;
	float z_near_viewmodel;
	float z_far_viewmodel;
	float aspect_ratio;
	float near_blur_depth;
	float near_focus_depth;
	float far_focus_depth;
	float far_blur_depth;
	float near_blur_radius;
	float far_blur_radius;
	float dof_quality;
	int motionblur_mode;
	char pad_0x45[0x44];
	int edge_blur;
};