#pragma once

template< typename ptr_type = uintptr_t >
struct address_base_t
{
	ptr_type m_ptr;

	address_base_t( ) : m_ptr{} {};
	address_base_t(ptr_type ptr) : m_ptr(ptr) {};
	address_base_t(ptr_type* ptr) : m_ptr(ptr_type(ptr)) {};
	address_base_t(void* ptr) : m_ptr(ptr_type(ptr)) {};
	address_base_t(const void* ptr) : m_ptr(ptr_type(ptr)) {};
	~address_base_t( ) = default;

	inline operator ptr_type( ) const
	{
		return m_ptr;
	}

	inline operator void* ( )
	{
		return reinterpret_cast<void*>(m_ptr);
	}

	inline ptr_type get_inner( ) const
	{
		return m_ptr;
	}

	template< typename t = address_base_t<ptr_type> >
	inline bool compare( t in ) const
	{
		return m_ptr == ptr_type(in);
	}

	
	inline address_base_t<ptr_type>& self_get( uint8_t in = 1 )
	{
		m_ptr = get<ptr_type>( in );
		return *this;
	}

	inline address_base_t<ptr_type>& self_offset( ptrdiff_t offset )
	{
		m_ptr += offset;
		return *this;
	}

	template< typename t = address_base_t<ptr_type> >
	inline address_base_t<ptr_type>& self_jmp( ptrdiff_t offset = 0x1 )
	{
		m_ptr = jmp( offset );
		return *this;
	}

	inline address_base_t<ptr_type>& self_find_opcode( byte opcode, ptrdiff_t offset = 0x0 )
	{
		m_ptr = find_opcode( opcode, offset );
		return *this;
	}

	inline address_base_t<ptr_type>& self_find_opcode_seq( std::vector<byte> opcodes, ptrdiff_t offset = 0x0 )
	{
		m_ptr = find_opcode_seq(opcodes, offset);

		return *this;
	}

	template< typename t = address_base_t<ptr_type> >
	inline address_base_t<ptr_type>& set( t in )
	{
		m_ptr = ptr_type( in );
		return *this;
	}

	
	template< typename t = ptr_type >
	inline t cast( )
	{
		return t( m_ptr );
	}

	template< typename t = address_base_t<ptr_type> >
	inline t get( uint8_t in = 1 )
	{
		ptr_type dummy = m_ptr;

		while ( in-- )
			if ( dummy )
				dummy = *reinterpret_cast<ptr_type*>( dummy );

		return t( dummy );
	}

	template< typename t = address_base_t<ptr_type> >
	inline t offset( ptrdiff_t offset )
	{
		return t( m_ptr + offset );
	}

	template< typename t = address_base_t<ptr_type> >
	inline t jmp( ptrdiff_t offset = 0x1 )
	{							
		ptr_type base = m_ptr + offset;
		auto displacement = *reinterpret_cast<int32_t*>( base );

		base += sizeof( uint32_t );
		base += displacement;

		return t(base);
	}

	template< typename t = address_base_t<ptr_type> >
	inline t find_opcode(byte opcode, ptrdiff_t offset = 0x0)
	{
		auto base = m_ptr;

		auto opcode_at_address = byte( );

		while (opcode_at_address = *reinterpret_cast<byte*>(base))
		{
			if (opcode == opcode_at_address)
				break;

			base += 1;
		}

		base += offset;
		return t(base);
	}

	template< typename t = address_base_t<ptr_type> >
	inline t find_opcode_seq(std::vector<byte> opcodes, ptrdiff_t offset = 0x0)
	{
		auto base = m_ptr;
		auto opcode_at_address = byte( );

		while (opcode_at_address = *reinterpret_cast<byte*>(base))
		{
			if (opcodes.at(0) == opcode_at_address)
			{
				for (auto i = 0u; i < opcodes.size( ); i++)
					if (opcodes.at(i) != *reinterpret_cast<byte*>(base + i))
						goto CONT;

				break;
			}
		CONT:
			base += 1;
		}

		base += offset;
		return t(base);
	}
};

using address_t = address_base_t<uintptr_t>;
using address_32_t = address_base_t<uint32_t>;
using address_64_t = address_base_t<uint64_t>;