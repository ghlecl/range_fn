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


//------------------------------------------------------------------------------
//! @brief Equivalent to [`std::enable_if_t`]
//! (http://en.cppreference.com/w/cpp/types/enable_if) in C++14 and up.
//!
//! As long as C++11 is the minimum supported version, this is necessary.
template< bool Boolean, class T = void >
using enable_if_t = typename std::enable_if< Boolean, T >::type;



//------------------------------------------------------------------------------
//! @brief Equivalent to [`std::remove_reference_t`]
//! (http://en.cppreference.com/w/cpp/types/remove_reference) in C++14 and up.
//!
//! As long as C++11 is the minimum supported version, this is necessary.
template< typename T >
using remove_reference_t = typename ::std::remove_reference< T >::type;



//------------------------------------------------------------------------------
//! Equivalent to [`std::remove_cv_t`]
//! (http://en.cppreference.com/w/cpp/types/remove_cv) in C++14 and up.
//!
//! As long as C++11 is the minimum supported version, this is necessary.
template< typename T >
using remove_cv_t = typename ::std::remove_cv< T >::type;

}


inline namespace cxx20 {

//------------------------------------------------------------------------------
//! Equivalent to [`std::remove_cvref`]
//! (http://en.cppreference.com/w/cpp/types/remove_cvref) in C++20 and up.
//!
//! As long as C++11 is the minimum supported version, this is necessary.
template< typename T >
struct remove_cvref {
   using type = ::estd::remove_cv_t< ::estd::remove_reference_t< T > >;
};


//! @brief Equivalent to [`std::remove_cvref_t`]
//! (http://en.cppreference.com/w/cpp/types/remove_cvref) in C++20 and up.
//!
//! As long as C++11 is the minimum supported version, this is
//! necessary.
template< typename T >
using remove_cvref_t = typename ::estd::remove_cvref<T>::type;

} // namespace cxx20



namespace detail {


//------------------------------------------------------------------------------
//! @brief Check if type is a character type
//!
//! Sets the value member of a std::integral_constant to true if the type `T` is
//! one of the following:
//!    - char
//!    - char16_t
//!    - char32_t
//!    - wchar_t
template< typename T >
using is_character = std::integral_constant<
   bool,
   std::is_same< ::estd::remove_cvref_t<T>, char >::value
   || std::is_same< ::estd::remove_cvref_t<T>, char16_t >::value
   || std::is_same< ::estd::remove_cvref_t<T>, char32_t >::value
   || std::is_same< ::estd::remove_cvref_t<T>, wchar_t >::value
>;




template< typename T >
using is_allowed_range_type = std::integral_constant<
   bool,
   ( std::is_integral< ::estd::remove_cvref_t<T> >::value
   && !std::is_same< ::estd::remove_cvref_t<T>, bool >::value )
   || std::is_floating_point< ::estd::remove_cvref_t<T> >::value
>;

template< typename Step >
using is_allowed_step_type = std::integral_constant<
   bool,
   ( is_allowed_range_type< Step >::value
      && !is_character< Step >::value )
>;




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
      static_assert(
         is_allowed_range_type<T>::value,
         "Only integers, characters and floating points are allowed in ranges."
      );
   }

   template< typename U, typename _ = void, typename = enable_if_t< length == Other, _ > >
   insist_inline Range( T start, T stop, U step ) :
      direction_{ (start < stop) ? Direction::ascending : Direction::descending },
      cur_val_{ start },
      end_{ stop },
      step_{ static_cast<T>(step) } {
      static_assert(
         detail::is_allowed_range_type<T>::value,
         "Only integers, characters and floating points are allowed in ranges."
      );
      static_assert(
         detail::is_allowed_step_type<U>::value,
         "Only integers and floating point types are allowed as the step type."
      );
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
   static_assert(
      detail::is_allowed_range_type<T>::value,
      "Only integers, characters and floating points are allowed in ranges."
   );
   return detail::unit_range<T>{ start, stop };
}



//------------------------------------------------------------------------------
template< typename T >
insist_inline auto range( T stop ) -> detail::unit_range< T > {
   static_assert(
      detail::is_allowed_range_type<T>::value,
      "Only integers, characters and floating points are allowed in ranges."
   );
   return range( T{0}, stop );
}



//------------------------------------------------------------------------------
template< typename T, typename U >
insist_inline auto range( T start, T stop, U step ) -> detail::range< T > {
   static_assert(
      detail::is_allowed_range_type<T>::value,
      "Only integers, characters and floating points are allowed in ranges."
   );
   static_assert(
      detail::is_allowed_step_type<U>::value,
      "Only integers and floating point types are allowed as the step type."
   );
   return detail::range< T >{ start, stop, step };
}

} // namespace estd

#endif // RANGE_FN_RANGE_HXX_
