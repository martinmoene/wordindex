 /*
 * Pair.h - token, line number pair type.
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

#ifndef _pair_h_included
#define _pair_h_included

namespace wordindex {

/**
 * pair type with converting constructors and converting operators.
 */
template < typename U, typename V >
struct Pair
{
   /**
    * default constructor.
    */
   Pair() : first(), second() { ; }

   /**
    * constructor.
    */
   Pair( U const& u, V const& v ) : first( u ), second( v ) { ; }

   /**
    * construct from first type.
    */
   explicit Pair( U const& u ) : first( u ), second() { ; }

   /**
    * construct from second type.
    */
   explicit Pair( V const& v ) : first(), second( v ) { ; }

   /**
    * assign from first type.
    */
   Pair& operator=( U const& u )
   {
      first = u;
      return *this;
   }

   /**
    * assign from second type.
    */
   Pair& operator=( V const& v )
   {
      second = v;
      return *this;
   }

   /**
    * convert to first type.
    */
   operator const U() const
   {
      return first;
   }

   /**
    * convert to second type.
    */
   operator const V() const
   {
      return second;
   }

   /**
    * first object.
    */
   U first;

   /**
    * second object.
    */
   V second;
};

} // namespace wordindex

#endif // _pair_h_included

/*
 * end of file
 */

