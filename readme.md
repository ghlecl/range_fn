# range_fn
A Python like range function to be used with C++11 range-based for loops.  The
code here accompanies a blog post on the subject.

## Usage ##
```c++
#include "range.hxx"
for( auto idx : estd::range( 8 ) ) { /* use idx */ }
for( auto idx : estd::range( 7, 1 ) ) { /* use idx */ }
for( auto idx : estd::range( 4, 32, 5 ) ) { /* use idx */ }
```
