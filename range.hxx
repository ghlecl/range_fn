//
// Copyright 2018 Ghyslain Leclerc
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef RANGE_FN_RANGE_HXX_
#define RANGE_FN_RANGE_HXX_

#include <cstdint>
#include <iterator>


namespace estd {

namespace detail {


//------------------------------------------------------------------------------
enum class Direction : uint_fast8_t {
   ascending,
   descending
};

constexpr auto Ascending = Direction::ascending;
constexpr auto Descending = Direction::descending;


//------------------------------------------------------------------------------
template< typename Iterator, typename Reference >
struct Dereference {
public:
   auto operator*() -> Reference {
      return static_cast<Iterator&>(*this).cur_val_;
   }
};



//------------------------------------------------------------------------------
template< typename Iterator >
struct Increment {
public:
   auto operator++() -> Iterator& {
      auto& self = static_cast<Iterator&>(*this);
      ( self.direction_ == Ascending ) ? ++(self.cur_val_): --(self.cur_val_);
      return static_cast<Iterator&>(*this);
   }
};



//------------------------------------------------------------------------------
template< typename Iterator >
struct EqualityComparisons {
   bool operator==( Iterator const& rhs ) const {
      return static_cast<Iterator const&>(*this).cur_val_ == rhs.cur_val_;
   }

   bool operator!=( Iterator const& rhs ) const {
      return !(*this == rhs);
   }
};



//------------------------------------------------------------------------------
template< typename T >
struct unit_range_iterator :
   Dereference< unit_range_iterator<T>, T& >,
   Increment< unit_range_iterator<T> >,
   EqualityComparisons< unit_range_iterator<T> > {
public:
   using value_type = T;
   using reference = T&;
   using iterator_category = std::input_iterator_tag;
   using pointer = T*;
   using difference_type = void;

   unit_range_iterator( T val, Direction dir )
               : direction_{ dir }, cur_val_{ val } {
   }
private:
   Direction direction_;
   T cur_val_;

   friend Dereference< unit_range_iterator<T>, T& >;
   friend Increment< unit_range_iterator<T> >;
   friend EqualityComparisons< unit_range_iterator<T> >;
};



//------------------------------------------------------------------------------
template< typename Iterator >
struct range {
private:
   using value_type = typename Iterator::value_type;
public:
   range( value_type start, value_type stop ) :
      direction_{ (start < stop) ? Ascending : Descending },
      cur_val_{ start },
      end_{ stop } {
   }

   auto begin() -> Iterator {
      return Iterator{ cur_val_, direction_ };
   }

   auto end() -> Iterator {
      return Iterator{ end_, direction_ };
   }

private:
   Direction direction_;
   value_type cur_val_;
   value_type end_;
};

} // namespace detail

} // namespace estd

#endif // RANGE_FN_RANGE_HXX_
