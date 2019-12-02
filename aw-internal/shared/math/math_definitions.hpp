#pragma once

#define PITCH 0
#define YAW 1
#define ROLL 2

namespace math
{
	inline float rad2deg( const float rad )
	{
		return static_cast<float>( rad * ( 180.f / M_PI ) );
	}

	inline float deg2rad( const float deg )
	{
		return static_cast< float >( deg * ( M_PI / 180.f ) );
	}

	inline void sincos( const float rad, float& sine, float& cosine )
	{
		sine = std::sin( rad );
		cosine = std::cos( rad );
	}
}