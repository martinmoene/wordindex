// g++ -I../../src -o insert_tokens.exe insert_tokens.cpp

#include <iostream>
#include <iterator>
#include "Tokenizer.h"
#include "WordIndex.h"

using wordindex::Tokenizer;
using wordindex::WordIndex;
using wordindex::wordindex_inserter;

namespace wordindex {

std::ostream& operator<< ( std::ostream& os, WordIndex::token_type const value )
{
   return os << "(" << value.first << "," << value.second << ")";
}

}

void insert_tokens( std::istream& is, WordIndex& map )
{
   Tokenizer tokenizer( is );

   std::copy
   ( tokenizer.begin()              // input collection begin
   , tokenizer.end()                // input collection end
   , wordindex_inserter( map )
   );
}

void print( std::ostream& os, WordIndex const& map )
{
//   std::copy
//   ( map.begin()
//   , map.end()
//   , std::ostream_iterator<WordIndex::token_type>( os, "\n" )
//   );
}

int main()
{
   WordIndex map;

   map.insert( WordIndex::token_type( "00_begin", 0 ) );

   insert_tokens( std::cin, map );

   map.insert( WordIndex::token_type( "ZZ_end", 0 ) );

   print( std::cout, map );
}
