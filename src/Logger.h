/*
 * src/Logger.h - message, warning and error logger class.
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

#ifndef logger_h_in_included
#define logger_h_in_included

#include <iostream>  // for std::cerr, std::ostream
#include <string>    // for std::string

namespace wordindex {

/**
 * message logger class.
 */
class Logger
{
public:
   /**
    * (default) constructor.
    */
   Logger( const std::string& progname, std::ostream& os = std::cout )
      : m_level   ( 0  )
      , m_os      ( os )
      , m_progname( progname )
   {
   }

   /**
    * destructor; close file, if required.
    */
   ~Logger()
   {
   }

   /**
    * the current logging level.
    */
   const int GetLevel() const
   {
      return m_level;
   }

   /**
    * set the loggin level.
    */
   void SetLevel( int const level )
   {
      m_level = level;
   }

   /**
    * report "programname: message" with given level.
    */
   void Report( int const level, std::string const& msg )
   {
      Report( level, m_progname, msg );
   }

   /**
    * report "filename: message" with given level.
    */
   void Report( int const level, std::string const& filename, std::string const& msg )
   {
      if ( level <= m_level )
      {
         m_os << filename << ": " << msg;
      }
   }

   /**
    * report "message" with given level.
    */
   void ReportPart( int const level, std::string const& msg )
   {
      if ( level <= m_level )
      {
         m_os << msg;
      }
   }

   /**
    * report a message with given level.
    */
   void Warning( std::string const& msg )
   {
      if ( &m_os != &std::cout
      &&   &m_os != &std::cerr
      &&   &m_os != &std::clog
      )
      {
         Report( 0, "Warning: " + msg );
      }
      std::cerr << m_progname << ": Warning: " << msg << std::endl;
   }

   /**
    * report a message with given level.
    */
   void Fatal( std::string const& msg )
   {
      if ( &m_os != &std::cout
      &&   &m_os != &std::cerr
      &&   &m_os != &std::clog
      )
      {
         Report( 0, msg );
      }
      std::cerr << m_progname << ": " << msg << std::endl;
      exit( 1 );
   }

private:
   /**
    * the level to report.
    */
   int m_level;

   /**
    * the output stream to write to.
    */
   std::ostream& m_os;

   /**
    * the program name.
    */
   std::string m_progname;
};

} // namespace wordindex

#endif //#ifndef logger_h_in_included

/*
 * end of file
 */

