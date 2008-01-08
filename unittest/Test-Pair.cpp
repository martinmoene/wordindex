/*
 * Test-Pair.cpp - test Pair.
 */

// VC6: cannot compile
// VC7: cl -GX -GR -I ../include Test-Pair.cpp
// GCC: C:\Programs\MinGW\bin\g++.exe -I ../include -o Test-Pair.exe Test-Pair.cpp

#include "../src/Pair.h"
#include <Fructose/test_base.h>

using wordindex::Pair;

struct test : public fructose::test_base< test >
{
   typedef int         first_type;
   typedef std::string second_type;

   typedef Pair< first_type, second_type > pair_type;

   first_type  first;
   second_type second;
   pair_type   pair;

   void setup()
   {
      first  = 33;
      second = "hello world";
      pair   = pair_type( first, second );
   }

   void teardown()
   {
      ; // do nothing
   }

   void is_proper_default_ctor( const std::string& test_name )
   {
      pair_type q;

      fructose_assert( first_type() == q.first && second_type() == q.second );
   }

   void is_proper_full_ctor( const std::string& test_name )
   {
      pair_type q( first, second );

      fructose_assert( first == q.first && second == q.second );
   }

   void is_proper_convert_from_first_ctor( const std::string& test_name )
   {
      pair_type q( first );
      // cannot convert from 'test::first_type' to 'wordindex::Pair<U,V>'
      // pair_type pair = first;

      fructose_assert( first == q.first && second_type() == q.second );
   }

   void is_proper_convert_from_second_ctor( const std::string& test_name )
   {
      pair_type q( second );
//      pair_type pair = second;

      fructose_assert( first_type() == q.first && second == q.second );
   }

   void is_proper_convert_from_first_assignment( const std::string& test_name )
   {
      pair_type q( second );

      q = first;

      fructose_assert( first == q.first && second == q.second );
   }

   void is_proper_convert_from_second_assignment( const std::string& test_name )
   {
      pair_type q( first );

      q = second;

      fructose_assert( first == q.first && second == q.second );
   }

   void is_proper_covert_to_first_operator( const std::string& test_name )
   {
      first_type x = pair;

      fructose_assert( first == x );
   }

   void is_proper_covert_to_second_operator( const std::string& test_name )
   {
      second_type x = pair;

      fructose_assert( second == x );
   }
};

int main( int argc, char* argv[] )
{
   test tests;
   tests.add_test( "is_proper_default_ctor"                  , &test::is_proper_default_ctor );
   tests.add_test( "is_proper_full_ctor"                     , &test::is_proper_full_ctor );
   tests.add_test( "is_proper_convert_from_first_ctor"       , &test::is_proper_convert_from_first_ctor );
   tests.add_test( "is_proper_convert_from_second_ctor"      , &test::is_proper_convert_from_second_ctor );
   tests.add_test( "is_proper_convert_from_first_assignment" , &test::is_proper_convert_from_first_assignment );
   tests.add_test( "is_proper_convert_from_second_assignment", &test::is_proper_convert_from_second_assignment );
   tests.add_test( "is_proper_covert_to_first_operator"      , &test::is_proper_covert_to_first_operator );
   tests.add_test( "is_proper_covert_to_second_operator"     , &test::is_proper_covert_to_second_operator );

   return tests.run( argc, argv );
}

/*
 * end of file
 */
