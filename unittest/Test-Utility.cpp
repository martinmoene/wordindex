/*
 * Test-WordIndex.cpp - test WordIndex.
 */

// VC6: cannot compile
// VC7: cl -GX -GR -I ../include Test-WordIndex.cpp
// GCC: C:\Programs\MinGW\bin\g++.exe -I ../include -o Test-WordIndex.exe Test-WordIndex.cpp

#include "../src/Utility.h"
#include <Fructose/test_base.h>

struct test : public fructose::test_base< test >
{
   void is_proper_( const std::string& test_name )
   {
      fructose_assert( 0 && "implement" );
   }
};

int main( int argc, char* argv[] )
{
   test tests;
   tests.add_test( "is_proper_", &test::is_proper_ );

   return tests.run( argc, argv );
}

/*
 * end of file
 */
