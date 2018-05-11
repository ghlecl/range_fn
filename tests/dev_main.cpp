#include <cstdio>

#include "../range.hxx"


int main( int argc, char* argv[] ) {
   for( auto idx : estd::range( 8 ) ) {
      printf( "%d, ", idx );
   }
   printf("\n");

   for( auto idx : estd::range( -5 ) ) {
      printf( "%d, ", idx );
   }
   printf("\n");

   for( auto idx : estd::range( 10, 17 ) ) {
      printf( "%d, ", idx );
   }
   printf("\n");

   for( auto idx : estd::range( 23, 17 ) ) {
      printf( "%d, ", idx );
   }
   printf("\n");

   return 0;
}
