#pragma once

#include "primitive_iterator.hpp"

namespace nlohmann
{
namespace detail
{
/*!
@brief an iterator value

@note This structure could easily be a union, but MSVC currently does not allow
unions members with complex constructors, see https://github.com/nlohmann/json/pull/105.
*/
template<typename BasicJsonType> struct internal_iterator
{
        typename BasicJsonType::object_t::iterator object_iterator {};
        typename BasicJsonType::array_t::iterator array_iterator {};
        primitive_iterator_t primitive_iterator {};
};
}  // namespace detail
}  // namespace nlohmann
