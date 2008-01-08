/*
 * Test-Tokenizer.cpp - test Tokenizer.
 */

// VC6: cannot compile
// VC7: cl -GX -GR -I ../include Test-Tokenizer.cpp
// GCC: C:\Programs\MinGW\bin\g++.exe -I ../include -o Test-Tokenizer.exe Test-Tokenizer.cpp

#include "../src/Tokenizer.h"
#include <Fructose/test_base.h>

using wordindex::Tokenizer;

const int number = 321;
const std::string text1 = "text";
const std::string text2 = "text2";
const std::string text3 = "_";
const std::string text4 = "_text";
const std::string text5 = "_text5";
const std::string text6 = "_123456";
const std::string text7 = "_1234text7";

struct test : public fructose::test_base< test >
{
   void is_working_iterator()
   {
   }

   void is_proper_text( const std::string& test_name )
   {
   }

   void is_proper_token_text( const std::string& test_name )
   {
      std::stringstream ss;

      ss <<
         number << " ~`!@# $%^ &*( ()-+= :;'\" ,./ <>? {}[]\\| " <<
         text1  << " " <<
         text2  << " " <<
         text3  << " " <<
         text4  << " " <<
         text5  << " " <<
         text6  << " " <<
         text7  << " " ;

      Tokenizer tokenizer( ss );

      Tokenizer::iterator pos( tokenizer.begin() );

      fructose_assert( ( pos != tokenizer.end() ) );

      fructose_assert( text1 == (*pos).first ); ++pos;
      fructose_assert( text2 == (*pos).first ); ++pos;
      fructose_assert( text3 == (*pos).first ); ++pos;
      fructose_assert( text4 == (*pos).first ); ++pos;
      fructose_assert( text5 == (*pos).first ); ++pos;
      fructose_assert( text6 == (*pos).first ); ++pos;
      fructose_assert( text7 == (*pos).first ); ++pos;

      fructose_assert( !( pos != tokenizer.end() ) );
   }
};

int main( int argc, char* argv[] )
{
   test tests;
   tests.add_test( "is_proper_token_text", &test::is_proper_token_text );

   return tests.run( argc, argv );
}

/*
 * end of file
 */
