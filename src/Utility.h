/*
 * src/utility.h - conversion shims etc.
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
 * $Id:$
 */

/**
 * \file
 */

#ifndef _utility_h_included
#define _utility_h_included

#include "Config.h"                     // for configuration

#include <io.h>                         // for _access()
#include <string>                       // for std::string
#include <sstream>                      // for std::stringstream

namespace wordindex {

/**
 * base class to implement UnCopyable interface.
 */
class UnCopyable
{
public:
   /**
    * default constructor.
    */
   UnCopyable() { ; }

private:
   /**
    * this class type.
    */
   typedef UnCopyable class_type;

   /**
    * \name Not To Be Implemented
    * @{
    */

   /// copy-assignment constructor.
   UnCopyable( class_type const& );

   /// copy-assignment operator.
   class_type& operator= ( class_type const& );

   /// @}
};

/**
 * string to character pointer conversion shim.
 */
inline const char *to_charptr( const std::string& s )
{
   return s.c_str();
}

/**
 * integer to string conversion shim.
 */
inline const std::string to_string( long const x )
{
   char buffer[32];

   return _ltoa( x, buffer, 10 );
}

/**
 * integer to string conversion shim.
 */
inline const std::string to_string( int const x )
{
   return to_string( static_cast< long >( x ) );
}

/**
 * integer to string conversion shim.
 */
inline const std::string to_string( double const x, int const n = 3 )
{
   char buffer[32];

   return _gcvt( x, n, buffer );
}

/**
 * convert version to string.
 */
inline const std::string to_version_string( int const version, int const div )
{
   int maj = version / div;
   int min = version - maj * div;

   return to_string( maj ) + "." + to_string( min );
}

/**
 * string to integer conversion shim.
 */
inline const long to_int( const char* s )
{
   return strtol( s, 0, 0 );
}

/**
 * string to real conversion shim.
 */
inline const double to_real( const char* s )
{
   return strtod( s, 0 );
}

/**
 * the filename without the extension.
 */
inline const std::string basename( std::string const filename )
{
   return filename.substr( 0, filename.find_last_of( "." ) );
}

// TODO (moene#1#): safe?
/**
 * the filename without the directory part and extension.
 */
inline const std::string filename( std::string const filename )
{
   std::string base( basename( filename ) );

   int pos = base.find_last_of( "/\\" );

   return base.substr( (0 != pos) + pos );
}

/**
 * the filename extension.
 */
inline const std::string extension( std::string const filename )
{
   return filename.substr( filename.find_last_of( "." ) );
}

/**
 * true if the specified file is "-" (stdin) or exists as diskfile, false otherwise.
 */
inline const bool exist( std::string const filename)
{
   return "-" == filename || 0 == _access( to_charptr( filename ), 0 );
}

} // namespace wordindex

#endif //#ifndef _utility_h_included

/*
 * end of file
 */

