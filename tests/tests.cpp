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

#include <vector>

#include "./catch.hpp"

#include "range.hxx"

//==============================================================================
SCENARIO( "Python like range function", "[range][iteration]" )
{
   GIVEN( "a 'range of int' object obtained using the estd::range function" )
   {
      WHEN( "it is used in a range based for loop with the auto variable" )
      {
         for( auto i : estd::range( 1 ) )
         {
            THEN( "the type of the auto variable is int." )
            {
               REQUIRE( std::is_same<decltype( i ), int>::value );
            }
         }
      }

      WHEN( "it is used in a range based for loop with the auto& variable" )
      {
         for( auto& i : estd::range( 1 ) )
         {
            THEN( "the type of the auto variable is int&." )
            {
               REQUIRE( std::is_same< decltype( i ), int& >::value );
            }
         }
      }

      WHEN( "it is used in a range based for loop with the auto const& variable" )
      {
         for( auto const& i : estd::range( 1 ) )
         {
            THEN( "the type of the auto variable is int const&." )
            {
               REQUIRE( std::is_same< decltype( i ), int const& >::value );
            }
         }
      }
   }


   GIVEN( "a 'range of unsigned int' object obtained using the estd::range function" )
   {
      WHEN( "it is used in a range based for loop with the auto variable" )
      {
         for( auto i : estd::range( 1u ) )
         {
            THEN( "the type of the auto variable is unsigned int." )
            {
               REQUIRE( std::is_same< decltype( i ), unsigned int >::value );
            }
         }
      }
   }


   GIVEN( "a 'range of long' object obtained using the estd::range function" )
   {
      WHEN( "it is used in a range based for loop with the auto variable" )
      {
         for( auto i : estd::range( 1l ) )
         {
            THEN( "the type of the auto variable is long." )
            {
               REQUIRE( std::is_same< decltype( i ), long >::value );
            }
         }
      }
   }


   GIVEN( "a 'range of double' object obtained using the estd::range function" )
   {
      WHEN( "it is used in a range based for loop with the auto& variable" )
      {
         for( auto& i : estd::range( 1.0 ) )
         {
            THEN( "the type of the auto variable is double&." )
            {
               REQUIRE( std::is_same< decltype( i ), double& >::value );
            }
         }
      }
   }


   GIVEN( "a number n of iterations to make and an empty vector" )
   {
      int n = 10;
      std::vector<int> the_vec;
      the_vec.reserve( n );
      WHEN( "range is called with that number to loop and fill the empty vector" )
      {
         for( auto& cur_idx : estd::range( n ) )
         {
            the_vec.push_back( cur_idx );
         }
         THEN( "the resulting vector is filled with numbers from 0 to n-1." )
         {
            REQUIRE( the_vec == std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
         }
      }
   }


   GIVEN( "a pair of numbers [b, e[ to iterate through" )
   {
      int b = 15;
      int e = 20;
      std::vector<int> the_vec;
      the_vec.reserve( 5 );
      WHEN( "range is called with those numbers" )
      {
         for( auto const& cur_idx : estd::range( b, e ) )
         {
            the_vec.push_back( cur_idx );
         }
         THEN( "the resulting vector is filled with numbers from b to e-1." )
         {
            REQUIRE( the_vec == std::vector<int>{ 15, 16, 17, 18, 19 } );
         }
      }
   }
}
