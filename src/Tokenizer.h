 /*
 * Tokenizer.h - tokenizer collection.
 *
 * This file is part of WordIndex.
 *
 * Copyright (C) 2007-2008, Martin J. Moene.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mngdriver; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id$
 */

/**
 * \file
 */

#include "Pair.h"     // for pair_type
#include "Utility.h"  // for class UnCopyable

#include <algorithm>   // for std::transform()
#include <cassert>     // for ::assert()

namespace wordindex {

/**
 * separate stream input into token, line number pairs.
 */
template < typename C >
class basic_tokenizer : private UnCopyable
{
public:
   /**
    * the character type.
    */
   typedef C char_type;

   /**
    * the class type.
    */
   typedef basic_tokenizer class_type;

   /**
    * the line number type.
    */
   typedef int line_number_type;

   /**
    * the token--line number pair type.
    */
   typedef Pair< std::string, line_number_type > value_type;

   /**
    * the iterator type.
    */
   class iterator : public std::iterator< std::input_iterator_tag, value_type >
   {
   public:
      /**
       * the class type.
       */
      typedef iterator class_type;

      /**
       * constructor.
       */
      explicit iterator( basic_tokenizer& tokenizer )
      : m_is       ( &tokenizer.m_is )
      , m_tokenizer( &tokenizer      )
      , m_line     (  1 )
      {
         operator++();
      }

      /**
       * default constructor.
       */
      iterator()
      : m_is       ( NULL )
      , m_tokenizer( NULL )
      , m_line     (  0   )
      {
      }

      /**
       * true if character is a valid comment start character.
       */
      const bool is_start_comment( char_type chr )
      {
          return ';' == chr || '#' == chr;
      }

      /**
       * true if character is a valid start character.
       */
      const bool is_start( char_type chr )
      {
          return '_' == chr || ::isalpha( chr );
      }

      /**
       * true if character is a valid second and following character.
       */
      const bool is_follow( char_type chr )
      {
          return '_' == chr
              || ::isalnum( chr )
              || std::string::npos != m_tokenizer->m_follow.find_first_of( chr );
      }

      /**
       * true if character is a valid start or following character.
       */
      const bool is_start_or_follow( char_type chr )
      {
          return is_start( chr ) || is_follow( chr );
      }

      /**
       * advance to next token.
       */
      class_type& operator++ ()
      {
         assert( NULL != m_is );

         char_type chr = ' ';
         m_token.erase();

         /*
          * skip non-start characters:
          */
         while ( *m_is && !is_start( chr = m_is->get() ) )
         {
            // skip line comments:
            if ( is_start_comment( chr ) && m_tokenizer->m_skip_comments )
            {
               while ( *m_is && '\n' != ( chr = m_is->get() ) )
               {
                  ;
               }
            }

            // count lines:
            if ( '\n' == chr )
            {
                ++m_line;
            };
         }
         m_is->putback( chr );

         /*
          * scan token:
          */
         while( *m_is && is_start_or_follow( chr = m_is->get() ) )
         {
            m_token.append( 1, chr );
         }
         m_is->putback( chr );

         /*
          * transform to lowercase?
          */
         if ( m_tokenizer->m_lowercase )
         {
            std::transform
            ( m_token.begin(), m_token.end()
            , m_token.begin()
            , tolower
            );
         }

         /*
          * signal end of input:
          */
         if ( ! *m_is )
         {
             m_is = NULL;
         }

         return *this;
      }

//      /**
//       * return current token string.
//       */
//       operator std::string() const
//      {
//         return m_token;
//      }
//
//      /**
//       * return current token string.
//       */
//      operator line_number_type() const
//      {
//         return m_line;
//      }

      /**
       * return current token string, line number pair.
       */
      const value_type operator*() const
      {
         return value_type( m_token, m_line );
      }

      /**
       * true if this and other iterators are unequal.
       */
      const bool operator!= ( class_type const& rhs ) const
      {
          return m_is != rhs.m_is;
      }

   private:
      /**
       * the input stream; NULL if end-of-input has been reached.
       */
      std::istream* m_is;

      /**
       * skip comments flag.
       */
      basic_tokenizer  const* m_tokenizer;

      /**
       * current token string.
       */
      std::string m_token;

      /**
       * current line number.
       */
      line_number_type m_line;
   };

   /**
    * constructor.
    */
   basic_tokenizer( std::istream& is )
   : m_is( is )
   , m_skip_comments( false )
   {
      ;
   }

   /**
    * return collection's begin iterator.
    */
   iterator begin()
   {
      return iterator( *this );
   }

   /**
    * return collection's end iterator.
    */
   iterator end()
   {
      return iterator();
   }

   /**
    * set or clear skip-comments flag.
    */
   void set_skip_comments( const bool skip = true )
   {
      m_skip_comments = skip;
   }

   /**
    * add given set of characters to the the start symbol set.
    */
   void add_to_start_set( std::string const& set )
   {
      m_start += set;
   }

   /**
    * add given set of characters to the the follow symbol set.
    */
   void add_to_followset( std::string const& set )
   {
      m_follow += set;
   }

   /**
    * set or clear the convert-to-lowercase flag.
    */
   void set_lowercase( const bool lowercase )
   {
      m_lowercase = lowercase;
   }

private:
   /**
    * the input stream pointer; NULL if end-of-input has been reached.
    */
   std::istream &m_is;

   /**
    * skip comments flag.
    */
   bool m_skip_comments;

   /**
    * transform tokens to lowercase.
    */
   bool m_lowercase;

   /**
    * additional follow set.
    */
   std::string m_start;

   /**
    * additional follow set.
    */
   std::string m_follow;
};

/**
 * tokenizer for char character type.
 */
typedef basic_tokenizer< char > Tokenizer;

} // namespace wordindex

/*
 * end of file
 */

