/*
 * Test-Fructose.cpp - simple test on fructose unit test framework.
 */

// VC6: cannot compile
// VC7: cl -GX -GR -I ../include Test-Fructose.cpp
// GCC: C:\Programs\MinGW\bin\g++.exe -I ../include -o Test-Fructose.exe Test-Fructose.cpp

//#include "../src/config.h" // correct for VC6 min(), max()
#include <fructose/test_base.h>

const int neighbour_of_the_beast = 668;

struct simpletest : public fructose::test_base< simpletest >
{
   void beast( const std::string& test_name )
   {
      fructose_assert( neighbour_of_the_beast == 668 )
   }
};

int main( int argc, char* argv[] )
{
   simpletest tests;
   tests.add_test( "beast", &simpletest::beast );

   return tests.run( argc, argv );
}

/*
 * end of file
 */
