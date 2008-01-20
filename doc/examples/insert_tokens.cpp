// g++ -I../../src -o insert_tokens.exe insert_tokens.cpp

#include <iostream>
#include <iterator>
#include "Tokenizer.h"
#include "WordIndex.h"

using wordindex::Tokenizer;
using wordindex::WordIndex;
using wordindex::wordindex_inserter;

void insert_tokens( std::istream& is, WordIndex& map )
{
   Tokenizer tokenizer( is );

   std::copy
   ( tokenizer.begin()              // input collection begin
   , tokenizer.end()                // input collection end
   , wordindex_inserter( map )      // convenience function that creates
   );                               //   the proper insert iterator
}

int main()
{
   WordIndex map;

   insert_tokens( std::cin, map );
}
