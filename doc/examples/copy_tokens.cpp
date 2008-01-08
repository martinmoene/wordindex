// g++ -I../../src -o copy_tokens.exe copy_tokens.cpp

#include <iostream>
#include <iterator>
#include "Tokenizer.h"

using wordindex::Tokenizer;

namespace wordindex {

std::ostream& operator<< ( std::ostream& os, Tokenizer::value_type const value )
{
   return os << "(" << value.first << "," << value.second << ")";
}

}

void copy_tokens( std::istream& is, std::ostream& os )
{
   Tokenizer tokenizer( is );

   std::copy
   ( tokenizer.begin()              // input collection begin
   , tokenizer.end()                // input collection end
   , std::ostream_iterator<Tokenizer::value_type>( os, "\n" )
   );
}

int main()
{
   copy_tokens( std::cin, std::cout );
}
