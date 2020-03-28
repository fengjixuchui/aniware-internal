#pragma once

struct bitflag_t
{
	bitflag_t( ) = default;
	bitflag_t(uintptr_t flags) : m_flags(flags) {}
	~bitflag_t( ) = default;

	uintptr_t m_flags{};

	inline bool has_flag(uintptr_t flag)
	{
		return m_flags & flag;
	}

	inline void add_flag(uintptr_t flag)
	{
		m_flags |= flag;
	}

	inline void remove_flag(uintptr_t flag)
	{
		m_flags &= ~flag;
	}

	inline bool is_empty( )
	{
		return !m_flags;
	}
};