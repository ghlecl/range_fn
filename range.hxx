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
enum class Length : uint_fast8_t {
   unit,
   other
};

constexpr const auto Unit = Length::unit;
constexpr const auto Other = Length::other;



//------------------------------------------------------------------------------
template< typename T, Length length, template< typename, Length > class Iterator >
struct Dereference
{
public:
   insist_inline
   auto operator*() -> T& {
      return static_cast< Iterator< T, length >& >(*this).cur_val_;
   }
};



//------------------------------------------------------------------------------
template< typename T, Length length, template< typename, Length > class Iterator >
struct Increment
{
public:
   template< typename Return = Iterator< T, length > >
   insist_inline
   auto operator++() -> enable_if_t< length == Unit, Return& > {
      auto& self = static_cast<Iterator< T, length >&>(*this);
      ( self.direction_ == Ascending ) ? ++(self.cur_val_): --(self.cur_val_);
      return static_cast<Iterator< T, length >&>(*this);
   }

   template< typename Return = Iterator< T, length > >
   insist_inline
   auto operator++() -> enable_if_t< length == Other, Return& > {
      static_cast<Iterator< T, length >&>(*this).cur_val_ +=
                           static_cast<Iterator< T, length >&>(*this).step_;
      return static_cast<Iterator< T, length >&>(*this);
   }
};



//------------------------------------------------------------------------------
template< typename T, Length length, template< typename, Length > class Iterator >
struct StopCondition
{
   template< typename Return = bool >
   insist_inline
   auto operator==( Iterator< T, length > const& rhs ) const
                  -> enable_if_t< !std::is_floating_point<T>::value
                                             && ( length == Unit), Return > {
      return static_cast<Iterator< T, length > const&>(*this).cur_val_ == rhs.cur_val_;
   }

   template< typename Return = bool >
   insist_inline
   auto operator==( Iterator< T, length > const& rhs ) const
                  -> enable_if_t< std::is_floating_point<T>::value
                                             || length == Other, Return > {
      return ( static_cast<Iterator< T, length > const&>(*this).direction_ == Ascending ) ?
               ( static_cast<Iterator< T, length > const&>(*this).cur_val_ >= rhs.cur_val_ ) :
                  ( static_cast<Iterator< T, length > const&>(*this).cur_val_ <= rhs.cur_val_ );
   }

   insist_inline
   bool operator!=( Iterator< T, length > const& rhs ) const {
      return !(*this == rhs);
   }
};



//------------------------------------------------------------------------------
template< typename T, Length length >
struct range_iterator;


template< typename T >
struct range_iterator< T, Unit > :
   Dereference< T, Unit, detail::range_iterator >,
   Increment< T, Unit, detail::range_iterator >,
   StopCondition< T, Unit, detail::range_iterator >
{
public:
   using value_type = T;
   using reference = T&;
   using iterator_category = std::input_iterator_tag;
   using pointer = T*;
   using difference_type = void;

   insist_inline
   range_iterator( T val, Direction dir )
               : direction_{ dir }, cur_val_{ val } {
   }
private:
   Direction const direction_;
   T cur_val_;

   friend Dereference< T, Unit, detail::range_iterator >;
   friend Increment< T, Unit, detail::range_iterator >;
   friend StopCondition< T, Unit, detail::range_iterator >;
};


template< typename T >
struct range_iterator< T, Other > :
   Dereference< T, Other, detail::range_iterator >,
   Increment< T, Other, detail::range_iterator >,
   StopCondition< T, Other, detail::range_iterator >
{
public:
   using value_type = T;
   using reference = T&;
   using iterator_category = std::input_iterator_tag;
   using pointer = T*;
   using difference_type = void;

   insist_inline
   range_iterator( T val, T step, Direction dir )
               : direction_{ dir }, cur_val_{ val }, step_{ step } {
   }
private:
   Direction direction_;
   T cur_val_;
   T step_;

   friend Dereference< T, Other, detail::range_iterator >;
   friend Increment< T, Other, detail::range_iterator >;
   friend StopCondition< T, Other, detail::range_iterator >;
};



//------------------------------------------------------------------------------
template< typename T, Length length, template< typename, Length > class Iterator >
struct Range
{
public:

   template< typename _ = void, typename = enable_if_t< length == Unit, _ > >
   insist_inline
   Range( T start, T stop ) :
      direction_{ (start < stop) ? Ascending : Descending },
      cur_val_{ start },
      end_{ stop },
      step_{1} {
   }

   template< typename U, typename _ = void, typename = enable_if_t< length == Other, _ > >
   insist_inline Range( T start, T stop, U step ) :
      direction_{ (start < stop) ? Direction::ascending : Direction::descending },
      cur_val_{ start },
      end_{ stop },
      step_{ static_cast<T>(step) } {
   }

   template< typename Return = Iterator<T, length> >
   insist_inline
   auto begin() -> enable_if_t< length == Unit, Return > {
      return Iterator< T, length >{ cur_val_, direction_ };
   }

   template< typename Return = Iterator<T, length> >
   insist_inline
   auto begin() -> enable_if_t< length == Other, Return > {
      return Iterator< T, length >{ cur_val_, step_, direction_ };
   }

   template< typename Return = Iterator<T, length> >
   insist_inline
   auto end() -> enable_if_t< length == Unit, Return > {
      return Iterator< T, length >{ end_, direction_ };
   }

   template< typename Return = Iterator<T, length> >
   insist_inline
   auto end() -> enable_if_t< length == Other, Return > {
      return Iterator< T, length >{ end_, step_, direction_ };
   }

private:
   Direction const direction_;
   T const cur_val_;
   T const end_;
   T const step_;
};


template< typename T >
using unit_range = Range< T, Unit, range_iterator >;

template< typename T >
using range = Range< T, Other, range_iterator >;


} // namespace detail



//------------------------------------------------------------------------------
template< typename T >
insist_inline auto range( T start, T stop ) -> detail::unit_range< T > {
   return detail::unit_range<T>{ start, stop };
}



//------------------------------------------------------------------------------
template< typename T >
insist_inline auto range( T stop ) -> detail::unit_range< T > {
   return range( T{0}, stop );
}



//------------------------------------------------------------------------------
template< typename T, typename U >
insist_inline auto range( T start, T stop, U step ) -> detail::range< T > {
   return detail::range< T >{ start, stop, step };
}

} // namespace estd

#endif // RANGE_FN_RANGE_HXX_
