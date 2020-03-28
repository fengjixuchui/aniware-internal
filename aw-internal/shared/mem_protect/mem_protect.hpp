#pragma once

struct mem_protect_t
{
							mem_protect_t(LPVOID address, uint32_t size, DWORD flags) : m_address(address), m_size(size), m_flags(0)
	{
		if (!VirtualProtect(m_address, m_size, flags, &m_flags))
			throw std::system_error(GetLastError( ), std::system_category( ), "Failed to protect the specified page-region");
	}

				~mem_protect_t( )
	{
		VirtualProtect(m_address, m_size, m_flags, &m_flags);
	}

				LPVOID m_address;

				uint32_t m_size;

				DWORD m_flags;
};

#define INIT_MEM_PROTECT_RW( address, size ) auto protect = mem_protect_t( address, size, PAGE_READWRITE );
#define INIT_MEM_PROTECT_R( address, size ) auto protect = mem_protect_t( address, size, PAGE_READONLY );
#define INIT_MEM_PROTECT_W( address, size ) auto protect = mem_protect_t( address, size, PAGE_WRITECOPY );
#define INIT_MEM_PROTECT( address, size, flags ) auto protect = mem_protect_t( address, size, flags );