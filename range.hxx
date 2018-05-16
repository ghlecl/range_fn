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


// Macro to help with insisting on inlining with the compiler
#if !(defined(insist_inline))

#  if defined( __clang__ ) || defined( __GNUC__ )
#     define insist_inline __attribute__((always_inline)) inline
#  elif defined( _MSC_VER ) // || COMPILER(RVCT))
#     define insist_inline __forceinline
#  else
#     define insist_inline inline
#  endif // compiler switch

#else // This code is not seen if our definition is active

#  ifdef _MSC_VER
#     pragma message(\
         "warning: insist_inline defined by compiler or library and might not \
behave as expected." \
      )
#  else
#     warning (                                                                \
         "insist_inline defined by compiler or library and might not behave as expected."\
      )
#  endif

#endif // insist_inline


namespace estd {

inline namespace cxx14 {

template< bool Boolean, class T = void >
using enable_if_t = typename std::enable_if< Boolean, T >::type;

}

namespace detail {


//------------------------------------------------------------------------------
enum class Direction : uint_fast8_t {
   ascending,
   descending
};

constexpr const auto Ascending = Direction::ascending;
constexpr const auto Descending = Direction::descending;



//------------------------------------------------------------------------------
template< typename Iterator, typename Reference >
struct Dereference {
public:
   insist_inline
   auto operator*() -> Reference {
      return static_cast<Iterator&>(*this).cur_val_;
   }
};



//------------------------------------------------------------------------------
template< typename Iterator >
struct Increment {
public:
   insist_inline
   auto operator++() -> Iterator& {
      auto& self = static_cast<Iterator&>(*this);
      ( self.direction_ == Ascending ) ? ++(self.cur_val_): --(self.cur_val_);
      return static_cast<Iterator&>(*this);
   }
};



//------------------------------------------------------------------------------

template< typename Iterator, typename Value >
struct EqualityComparisons {

   template< typename T = bool >
   insist_inline
   auto operator==( Iterator const& rhs ) const
                  -> enable_if_t< !std::is_floating_point<Value>::value, T > {
      return static_cast<Iterator const&>(*this).cur_val_ == rhs.cur_val_;
   }

   template< typename T = bool >
   insist_inline
   auto operator==( Iterator const& rhs ) const
                  -> enable_if_t< std::is_floating_point<Value>::value, T > {
      return ( static_cast<Iterator const&>(*this).direction_ == Ascending ) ?
               ( static_cast<Iterator const&>(*this).cur_val_ >= rhs.cur_val_ ) :
                  ( static_cast<Iterator const&>(*this).cur_val_ <= rhs.cur_val_ );
   }

   insist_inline
   bool operator!=( Iterator const& rhs ) const {
      return !(*this == rhs);
   }
};



//------------------------------------------------------------------------------
template< typename T >
struct unit_range_iterator :
   Dereference< unit_range_iterator<T>, T& >,
   Increment< unit_range_iterator<T> >,
   EqualityComparisons< unit_range_iterator<T>, T > {
public:
   using value_type = T;
   using reference = T&;
   using iterator_category = std::input_iterator_tag;
   using pointer = T*;
   using difference_type = void;

   insist_inline
   unit_range_iterator( T val, Direction dir )
               : direction_{ dir }, cur_val_{ val } {
   }
private:
   Direction const direction_;
   T cur_val_;

   friend Dereference< unit_range_iterator<T>, T& >;
   friend Increment< unit_range_iterator<T> >;
   friend EqualityComparisons< unit_range_iterator<T>, T >;
};



//------------------------------------------------------------------------------
template< typename Iterator >
struct range {
private:
   using value_type = typename Iterator::value_type;
public:
   insist_inline
   range( value_type start, value_type stop ) :
      direction_{ (start < stop) ? Ascending : Descending },
      cur_val_{ start },
      end_{ stop } {
   }

   insist_inline
   auto begin() -> Iterator {
      return Iterator{ cur_val_, direction_ };
   }

   insist_inline
   auto end() -> Iterator {
      return Iterator{ end_, direction_ };
   }

private:
   Direction const direction_;
   value_type const cur_val_;
   value_type const end_;
};


template< typename T >
using unit_range = range< unit_range_iterator<T> >;

} // namespace detail


template< typename T >
insist_inline auto range( T start, T stop ) -> detail::unit_range<T> {
   return detail::unit_range<T>{ start, stop };
}

template< typename T >
insist_inline auto range( T stop ) -> detail::unit_range<T> {
   return range( T{0}, stop );
}

} // namespace estd

#endif // RANGE_FN_RANGE_HXX_
