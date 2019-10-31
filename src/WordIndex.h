/*
 * wordindex.h - word index collection and - insert iterator.
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

#ifndef wordindex_h_included
#define wordindex_h_included

#include "Pair.h"    // for class wordindex::Pair<>
#include "Utility.h" // for class wordindex::UnCopyable

#include <map>       // for std::map<> (associative array)
#include <vector>    // for std::vector (list if line numbers)
#include <string>    // for std::string

namespace wordindex {

// TODO (Martin#1#): localized sorting?
/**
 * std::string comparator for use in map.
 */
class StringLess
{
public:
  /**
   * apply.
   */
   virtual const bool operator()( std::string const& s1, std::string const& s2 ) const
   {
      return s1 < s2;
   }
};

/**
 * std::string comparator for use in map.
 */
class NoCaseLess : public StringLess
{
public:
  /**
   * apply.
   */
   virtual const bool operator()( std::string const& s1, std::string const& s2 ) const
   {
      return std::lexicographical_compare
      (
         s1.begin(), s1.end(),
         s2.begin(), s2.end(),
         nocase_less
      );
   }

private:
  /**
   * character comparator for std::lexicographical_compare().
   */
   static const bool nocase_less( char const c1, char const c2 )
   {
       return toupper( c1 ) < toupper( c2 );
   }
};

/**
 * collect tokens with their associated line numbers.
 */
class WordIndex : private UnCopyable
{
private:
   /**
    * the word type.
    */
   typedef std::string word_type;

   /**
    * the line number type.
    */
   typedef int line_number_type;

   /**
    * the list of line numbers type.
    */
   typedef std::vector< line_number_type > locations_type;

   /**
    * the token--locations associative array (map).
    */
   typedef std::map< word_type, locations_type, StringLess > map_type;

public:
   /**
    * this class type.
    */
   typedef WordIndex class_type;

   /**
    * this value type.
    */
   typedef map_type::value_type value_type;

   /**
    * the token--line number pair type.
    */
   typedef Pair< word_type, line_number_type > token_type;

   /**
    * the string--line numbers map iterator type.
    */
   typedef map_type::iterator iterator;

   /**
    * the string--line numbers map const iterator type.
    */
   typedef map_type::const_iterator const_iterator;

   /**
    * constructor.
    */
   WordIndex()
   : m_lines( 0 )
   , m_words( NoCaseLess() )
   {
      ;
   }

   /**
    * const begin iterator.
    */
   const_iterator begin() const
   {
      return const_begin();
   }

   /**
    * const begin iterator.
    */
   const_iterator const_begin() const
   {
      return m_words.begin();
   }

   /**
    * const end iterator.
    */
   const_iterator end() const
   {
      return const_end();
   }

   /**
    * const end iterator.
    */
   const_iterator const_end() const
   {
      return m_words.end();
   }

   /**
    * add a token, line number pair.
    */
   void insert( token_type const& pair )
   {
      insert( pair.first, pair.second );
   }

   /**
    * add a token, line number pair.
    */
   void insert( iterator pos, token_type const& pair )
   {
      insert( pair.first, pair.second );
   }

   /**
    * add a token and line number.
    */
   void insert( std::string const s, line_number_type const n )
   {
      ++m_lines;
      m_words[ s ].push_back( n );
   }

   /**
    * number of distinct words.
    */
   const int words() const
   {
       return m_words.size();
   }

   /**
    * number of line references.
    */
   const int lines() const
   {
      return m_lines;
   }

private:
   /**
    * number of line references.
    */
   int m_lines;

   /**
    * datastructure: a map of token--list of linenumbers pairs (associative array).
    */
   map_type m_words;
};

/**
* insert iterator.
*/
#if defined( _WORDINDEX_MSC6 )
template < typename C >
class insert_iterator : public std::iterator< std::output_iterator_tag, void >
#else
template < typename C >
class insert_iterator : public std::iterator< std::output_iterator_tag, void, void, void, void >
#endif
{
public:
  /**
   * the container type.
   */
  typedef C container_type;

  /**
   * this class type.
   */
  typedef insert_iterator class_type;

  /**
   * the value type.
   */
  typedef typename container_type::token_type value_type;

  /**
   * constructor.
   */
  explicit insert_iterator( container_type& container )
  : m_container( container )
  {
     ;
  }

  /**
   * pre-increment (no-op).
   */
  class_type& operator++()
  {
     return *this;
  }

  /**
   * post-increment (no-op).
   */
  class_type& operator++( int )
  {
     return *this;
  }

  /**
   * de-reference.
   */
  class_type& operator* ()
  {
     return *this;
  }

  /**
   * insert (add) a value.
   */
  class_type& operator=( value_type const& value )
  {
     m_container.insert( value );
     return *this;
  }

private:
  /**
   * the container.
   */
  container_type& m_container;
};

/**
 * convenience function to create WordIndex insert_iterator.
 */
inline insert_iterator< WordIndex > wordindex_inserter( WordIndex& collection )
{
    return insert_iterator< WordIndex >( collection );
}

} // namespace wordindex

#endif // wordindex_h_included

/*
 * end of file
 */
