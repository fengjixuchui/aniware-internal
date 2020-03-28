#pragma once

#include <cstddef> // ptrdiff_t
#include <iterator> // reverse_iterator
#include <utility> // declval

namespace nlohmann
{
namespace detail
{
// reverse_iterator //

/*!
@brief a template for a reverse iterator class

@tparam Base the base iterator type to reverse. Valid types are @ref
iterator (to create @ref reverse_iterator) and @ref const_iterator (to
create @ref const_reverse_iterator).

@requirement The class satisfies the following concept requirements:
-
[BidirectionalIterator](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator):
  The iterator that can be moved can be moved in both directions (i.e.
  incremented and decremented).
- [OutputIterator](https://en.cppreference.com/w/cpp/named_req/OutputIterator):
  It is possible to write to the pointed-to element (only if @a Base is
  @ref iterator).

@since version 1.0.0
*/
template<typename Base>
class json_reverse_iterator : public std::reverse_iterator<Base>
{
  public:
    using difference_type = std::ptrdiff_t;
        using base_iterator = std::reverse_iterator<Base>;
        using reference = typename Base::reference;

        explicit json_reverse_iterator(const typename base_iterator::iterator_type& it) noexcept
        : base_iterator(it) {}

        explicit json_reverse_iterator(const base_iterator& it) noexcept : base_iterator(it) {}

        json_reverse_iterator const operator++(int)
    {
        return static_cast<json_reverse_iterator>(base_iterator::operator++(1));
    }

        json_reverse_iterator& operator++( )
    {
        return static_cast<json_reverse_iterator&>(base_iterator::operator++( ));
    }

        json_reverse_iterator const operator--(int)
    {
        return static_cast<json_reverse_iterator>(base_iterator::operator--(1));
    }

        json_reverse_iterator& operator--( )
    {
        return static_cast<json_reverse_iterator&>(base_iterator::operator--( ));
    }

        json_reverse_iterator& operator+=(difference_type i)
    {
        return static_cast<json_reverse_iterator&>(base_iterator::operator+=(i));
    }

        json_reverse_iterator operator+(difference_type i) const
    {
        return static_cast<json_reverse_iterator>(base_iterator::operator+(i));
    }

        json_reverse_iterator operator-(difference_type i) const
    {
        return static_cast<json_reverse_iterator>(base_iterator::operator-(i));
    }

        difference_type operator-(const json_reverse_iterator& other) const
    {
        return base_iterator(*this) - base_iterator(other);
    }

        reference operator[](difference_type n) const
    {
        return *(this->operator+(n));
    }

        auto key( ) const -> decltype(std::declval<Base>( ).key( ))
    {
        auto it = --this->base( );
        return it.key( );
    }

        reference value( ) const
    {
        auto it = --this->base( );
        return it.operator * ( );
    }
};
}  // namespace detail
}  // namespace nlohmann
