#include <cstdio>

#include "../range.hxx"


int main( int argc, char* argv[] ) {
   using namespace estd::detail;
   for( auto idx : range< unit_range_iterator<int> >{ 0, 8 } ) {
      printf( "%d, ", idx );
   }
   printf("\n");
   return 0;
}
