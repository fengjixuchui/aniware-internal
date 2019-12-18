#pragma once

struct stack_frame_t
{
	address_t m_fp;
	address_t m_ret;

	stack_frame_t() = default;
	stack_frame_t( void* ret ) : m_fp{} { setup( uintptr_t( ret ) ); }
	stack_frame_t( uintptr_t ret ) : m_fp{} { setup( ret ); }
	~stack_frame_t() = default;

	void setup( uintptr_t ret )
	{
		m_fp = address_t( ret - sizeof( uintptr_t ) );
		m_ret = address_t( ret ).get();
	}

	uintptr_t get_frame_pointer()
	{
		return m_fp;
	}

	uintptr_t get_return_address()
	{
		return m_ret;
	}

	void previous()
	{
		m_fp.self_get();
	}

	template< typename t = uintptr_t >
	t get_var( std::ptrdiff_t offset ) 
	{
		return m_fp.offset( offset ).cast< t >();
	}
};

#define INIT_STACKFRAME() auto stack_frame = stack_frame_t( uintptr_t( _AddressOfReturnAddress() ) );
