#pragma once

namespace math
{
	struct vec3_t
	{
		vec3_t( ) = default;

		vec3_t( float xyz ) : x( xyz ), y( xyz ), z( xyz ) {};
		vec3_t( float x, float y ) : x( x ), y( y ), z( 0 ) {};
		vec3_t( float x, float y, float z ) : x( x ), y( y ), z( z ) {};
		vec3_t( float* arr ) : x( arr[ PITCH ] ), y( arr[ YAW ] ), z( arr[ ROLL ] ) {};

		inline float length_sqr( ) const
		{
			return ( ( x * x ) + ( y * y ) + ( z * z ) );
		}

		inline float length( ) const
		{
			return sqrt( length_sqr( ) );
		}

		inline float length_2d( ) const
		{
			float root = 0.0f;
			float sqst = x * x + y * y;

			__asm
			{
				sqrtss xmm0, sqst
				movss root, xmm0
			}

			return root;
		}

		inline vec3_t normalize( ) const
		{
			vec3_t out{};
			float l = length( );

			if ( l != 0 )
			{
				out.x = x / l;
				out.y = y / l;
				out.z = z / l;
			}
			else
				out.x = out.y = 0.0f; out.z = 1.0f;

			return out;
		}

		inline vec3_t normalized( ) const
		{
			vec3_t out( *this );

			out.normalize( );

			return out;
		}

		inline float self_dot( ) const
		{
			return ( x * x + y * y + z * z );
		}
	
		inline bool valid( ) const
		{
			return std::isfinite< float >( x ) 
				&& std::isfinite< float >( y ) 
				&& std::isfinite< float >( z );
		}

		inline bool zero( ) const
		{
			return x == 0.f
				&& y == 0.f
				&& z == 0.f;
		}

		inline float dot( const vec3_t & in ) const
		{
			return ( x * in.x + y * in.y + z * in.z );
		}

		inline float dot( const float* in ) const
		{
			return ( x * in[ PITCH ] + y * in[ YAW ] + z * in[ ROLL ] );
		}

#pragma region assignment
		inline vec3_t& operator=( const vec3_t & in )
		{
			x = in.x;
			y = in.y;
			z = in.z;

			return *this;
		}
#pragma endregion assignment

#pragma region equality
		inline bool operator!=( const vec3_t & in )
		{
			return ( x != in.x || y != in.y || z != in.z );
		}

		inline bool operator==( const vec3_t & in )
		{
			return ( x == in.x && y == in.y && z == in.z );
		}
#pragma endregion equality

#pragma region addition
		inline vec3_t operator+( vec3_t in ) const
		{
			return vec3_t(
				x + in.x,
				y + in.y,
				z + in.z
			);
		}

		inline vec3_t operator+( float in ) const
		{
			return vec3_t(
				x + in,
				y + in,
				z + in
			);
		}

		inline vec3_t& operator+=( vec3_t in )
		{
			x += in.x;
			y += in.y;
			z += in.z;

			return *this;
		}

		inline vec3_t& operator+=( float in )
		{
			x += in;
			y += in;
			z += in;

			return *this;
		}
#pragma endregion addition

#pragma region substraction
		inline vec3_t operator-( vec3_t in ) const
		{
			return vec3_t(
				x - in.x,
				y - in.y,
				z - in.z
			);
		}

		inline vec3_t operator-( float in ) const
		{
			return vec3_t(
				x - in,
				y - in,
				z - in
			);
		}

		inline vec3_t& operator-=( vec3_t in )
		{
			x -= in.x;
			y -= in.y;
			z -= in.z;

			return *this;
		}

		inline vec3_t& operator-=( float in )
		{
			x -= in;
			y -= in;
			z -= in;

			return *this;
		}
#pragma endregion substraction

#pragma region multiplication
		inline vec3_t operator*( vec3_t in ) const
		{
			return vec3_t(
				x * in.x,
				y * in.y,
				z * in.z
			);
		}

		inline vec3_t operator*( float in ) const
		{
			return vec3_t(
				x * in,
				y * in,
				z * in
			);
		}

		inline vec3_t& operator*=( vec3_t in )
		{
			x *= in.x;
			y *= in.y;
			z *= in.z;

			return *this;
		}

		inline vec3_t& operator*=( float in )
		{
			x *= in;
			y *= in;
			z *= in;

			return *this;
		}
#pragma endregion multiplication

#pragma region division
		inline vec3_t operator/( vec3_t in ) const
		{
			return vec3_t(
				x / in.x,
				y / in.y,
				z / in.z
			);
		}

		inline vec3_t operator/( float in ) const
		{
			return vec3_t(
				x / in,
				y / in,
				z / in
			);
		}

		inline vec3_t& operator/=( vec3_t in )
		{
			x /= in.x;
			y /= in.y;
			z /= in.z;

			return *this;
		}

		inline vec3_t& operator/=( float in )
		{
			x /= in;
			y /= in;
			z /= in;

			return *this;
		}
#pragma endregion division

		float x, y, z;
	};

	struct __declspec( align( 16 ) ) vec3_aligned_t : public vec3_t
	{
		inline vec3_aligned_t &operator=( const vec3_t& vec )
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;

			return *this;
		}

		float w;
	};
}