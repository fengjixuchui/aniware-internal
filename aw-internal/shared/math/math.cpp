#include "../shared.hpp"

namespace math
{
	vec3_t calc_angle( const vec3_t& src, const vec3_t& dst )
	{
		vec3_t delta = src - dst;
		double hyp = std::sqrtf( delta.x * delta.x + delta.y * delta.y );

		vec3_t ang;
		ang.x = static_cast< float >( std::atan( delta.z / hyp ) * ( 180.0 / M_PI ) );
		ang.y = static_cast< float >( std::atan( delta.y / delta.x ) * ( 180.0 / M_PI ) );
		ang.z = 0.f;

		if (delta.x >= 0.f)
			ang.y += 180.f;

		normalize_angle(ang);
		return ang;
	}

	float random( const float min, const float max )
	{
		return min + ( ( static_cast<float>( std::rand() ) / static_cast<float>( RAND_MAX ) ) * ( max - min ) );
	}

	void normalize_angle( vec3_t& ang )
	{
		ang.x = std::isfinite( ang.x ) ? std::remainderf( ang.x, 360.0f ) : 0.0f;
		ang.y = std::isfinite( ang.y ) ? std::remainderf( ang.y, 360.0f ) : 0.0f;
		ang.z = 0.0f;
	}

	void normalize_angle( float &ang )
	{
		if ( ang > 180.f || ang < -180.f )
		{
			float revolutions = std::round( std::abs( ang / 360.f ) );
			ang < 0.f ? ang = ang + 360.f * revolutions : ang = ang - 360.f * revolutions;
		}
	}

	void angle_vectors( const vec3_t& ang, vec3_t& out_forward )
	{
		auto sine = vec2_t(),
			cosine = vec2_t();

		sincos( deg2rad( ang.x ), sine.x, cosine.x );
		sincos( deg2rad( ang.y ), sine.y, cosine.y );

		out_forward.x = cosine.x * cosine.y;
		out_forward.y = cosine.x * sine.y;
		out_forward.z = -sine.x;
	}

	void angle_vectors( const vec3_t& ang, vec3_t* out_forward, vec3_t* out_right, vec3_t* out_up )
	{
		auto sine = vec3_t(),
			cosine = vec3_t();

		sincos( deg2rad( ang.x ), sine.x, cosine.x );
		sincos( deg2rad( ang.y ), sine.y, cosine.y );
		sincos( deg2rad( ang.z ), sine.z, cosine.z );

		out_forward->x = cosine.x * cosine.y;
		out_forward->y = cosine.x * sine.y;
		out_forward->z = -sine.x;

		out_right->x = ( -1 * sine.z * sine.x * cosine.y + -1 * cosine.z * -sine.y );
		out_right->y = ( -1 * sine.z * sine.x * sine.y + -1 * cosine.z * cosine.y );
		out_right->z = ( -1 * sine.z * cosine.x );

		out_up->x = ( cosine.z * sine.x * cosine.y + -sine.z * -sine.y );
		out_up->y = ( cosine.z * sine.x * sine.y + -sine.z * cosine.y );
		out_up->z = ( cosine.z * cosine.x );
	}

	void vector_angles( const vec3_t& vec, vec3_t& out )
	{
		if ( vec.x == 0.f && vec.y == 0.f )
		{
			out.x = vec.z > 0.f ? -90.f : 90.f;
			out.y = 0.f;
		}
		else
		{
			out.x = rad2deg( std::atan2( -vec.z, vec.length_2d() ) );
			out.y = rad2deg( std::atan2( vec.y, vec.x ) );
		}

		out.z = 0.f;
	}

	void vector_transform( const vec3_t & in_vec, const matrix3x4_t & in_mat, vec3_t & out_vec )
	{
		out_vec.x = in_vec.dot( in_mat[ 0 ] ) + in_mat[ 0 ][ 3 ];
		out_vec.y = in_vec.dot( in_mat[ 1 ] ) + in_mat[ 1 ][ 3 ];
		out_vec.z = in_vec.dot( in_mat[ 2 ] ) + in_mat[ 2 ][ 3 ];
	}
}