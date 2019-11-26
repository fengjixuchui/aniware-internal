#pragma once

#include <cstddef> // size_t

namespace nlohmann
{
namespace detail
{
struct position_t
{
        std::size_t chars_read_total = 0;
        std::size_t chars_read_current_line = 0;
        std::size_t lines_read = 0;

        constexpr operator size_t() const
    {
        return chars_read_total;
    }
};

} // namespace detail
} // namespace nlohmann
