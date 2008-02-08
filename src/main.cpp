/*
 * main.cpp - program's main line.
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

/*
 * compile: cl /EHsc -I../include -Fewordindex.exe main.cpp
 * compile: g++ -I../include -o wordindex.exe main.cpp
 */

/*
 *   keywords: 33
 *      words: 1
 * references: 2
 *
 *      hello: 100% (2)  5 33
 */

/*
 * handle options:
 * ideas:
 * - case sensitivity +case=(preserve,ignore) [preserve]
 * - number of lines per page +pagelength=n [1]
 * - tokens to accept
 * - keyword tokens (stop-words) +stopwords=filename [none]
 *
 * report:
 * - report-summary: keywords, words, references
 * - report-xxx:
 * - number of distinct keywords (stopwords): +report=count_keywords
 * - number of distinct words: +report=count_words
 * - number of word occurrences: +report=count_keywords
 * - percentage of word occurrences
 * - lines with word occurrences: +report=
 * - shorthand: +report=count_words,...
 */

/**
 * \file
 */

#include "Config.h"     // for configuration
#include "Logger.h"     // for class Logger
#include "Pair.h"       // for pair_type
#include "Tokenizer.h"  // for class Tokenizer
#include "Utility.h"    // shims
#include "Version.h"    // for WORDINDEX_VERSION_STRING
#include "WordIndex.h"  // for class WordIndex

#include <ctype.h>     // for ::isalpha()

#include <algorithm> // for std::copy()
#include <set>       // for std::ste<> (associative array)
#include <iterator>  // for std::iterator<> base class
#include <iomanip>   // for std::setw() etc.
#include <fstream>   // for std::ifstream
#include <iostream>  // for std::cin, std::cout
#include <string>    // for std::string
//#include <utility>   // for std::pair<>
#include <vector>    // for std::vector (list if line numbers)

#include <tclap/CmdLine.h>    // for templatized C++ command line parser library

/**
 * the dimension of an array.
 */
#define dimensionof( a ) ( sizeof(a) / sizeof( *(a) ) )

/**
 * clp namespace shorthand for TCLAP.
 */
namespace clp = TCLAP;

/**
 * namespace for this application.
 */
namespace wordindex {

/**
 * \name Configuration
 * @{
 */

/**
 * the author name.
 */
std::string const author_string( "Martin J. Moene <m.j.moene@eld.physics.LeidenUniv.nl>" );

/**
 * the program version.
 */
std::string const version_string( WORDINDEX_RELEASE_NUMBER_STRING );

/**
 * the program date.
 */
std::string const date_string   ( WORDINDEX_RELEASE_DATE_STRING );

/**
 * the program name / basename.
 */
std::string /* const */ program_name( "wordindex" );

/**
 * the program banner.
 */
std::string const banner(
   program_name + "  " + version_string + " (" + date_string + ")  create alphabetically sorted index of words.\n\n"
);

/**
 * the program copyright.
 */
std::string const copyright(
   "Copyright 2007-2008, by Martin J. Moene.\n"
);

/// @} Configuration-End

/**
 * hint to read help.
 */
std::string const try_help( "Try '" + program_name + " --help' for more information.\n" );

/**
 * the line number type.
 */
typedef int line_number_type;

/**
 * the word type.
 */
typedef std::string word_type;

/**
 * the filename type.
 */
typedef std::string filename_type;

/**
 * the keyword container.
 */
typedef std::set< word_type > keyword_collection_type;

/**
 * the keyword container.
 */
typedef keyword_collection_type Keywords;

/**
 * the filename list element type.
 */
typedef Pair< filename_type, line_number_type > filename_list_element_type;

/**
 * the filename list type.
 */
typedef std::vector< filename_list_element_type > filename_list_type;

/**
 * print collections contents.
 */
template < typename C >
void print_collection( std::ostream& os, C const& collection )
{
   std::copy
   ( collection.begin()
   , collection.end()
   , std::ostream_iterator< typename_type_k C::value_type >( os, "  " )
   );
}

/**
 * the logger instance (external linkage).
 */
Logger logger( program_name, std::cout );

/**
 * the program usage.
 */
int usage( std::ostream& os, int status )
{
   os <<
//      banner <<
      "Usage: " << program_name << " [option...] [file...]\n"
      "\n"
      "  -h, --help          display this help and exit\n"
      "  -a, --author        report authors name and e-mail [no]\n"
      "      --version       report program and compiler versions [no]\n"
      "  -v, --verbose       report ... [none]\n"
      "\n"
      "  -f, --frequency     also report word frequency as d.dd% (n) [no]\n"
//      "  -g, --ignorecase    handle upper and lowercase as being equivalent [no]\n"
      "  -l, --lowercase     transform words to lowercase [no]\n"
      "  -r, --reverse       only collect keyword occurrences, see --keywords [no]\n"
      "  -s, --summary       also report number of (key)words and references [no]\n"
      "\n"
      "  -i, --input=file    read filenames from given file [standard input or given filenames]\n"
      "  -o, --output=file   write output to given file [standard output]\n"
      "  -k, --keywords=file read keywords to skip (stopwords) from given file [none]\n"
      "\n"
      "Long options also may start with a plus, like: +help.\n"
      "\n" <<
      filename( program_name )  << " creates an alphabetically sorted index of words present in the\n"
      "input files and it reports the lines where those words occur.\n"
      "Words that are marked as keywords are excluded (see option --keywords).\n"
      "Use option --reverse to only show the occurrences of keywords.\n"
      "\n"
      "Words can be read from standard input, or from files specified on the command\n"
      "line and from files that are specified in another file (see option --input).\n"
      "\n"
      "A file that specifies input filenames may look as follows:\n"
      "   # comment that extends to the end of the line ( ; also starts comment line)\n"
      "   file1.txt file2.txt\n"
      "   file3.txt\n"
      "\n"
      "Example:\n"
      "   echo hello world | " << filename( program_name ) << " +summary +frequency\n"
      "       keywords  0\n"
      "          words  2\n"
      "     references  2\n"
      "\n"
      "          hello  50% (1)  1\n"
      "          world  50% (1)  1\n"
      "\n"
      "Example:\n"
      "   " << filename( program_name )  << " +lowercase file.txt | sort -n -k2 -r \n"
      "This creates a list of lowercase words, sorted on frequency of occurrence.\n"
      "\n"
//      << copyright
      ;

   return status;
}

/**
 * the version help.
 */
int version( std::ostream& os, int status )
{
   os <<
      banner <<
      "Compiled with "
#if defined( __GNUC__ )
      "GNU C++ version "  << __GNUC__ << "." << __GNUC_MINOR__ <<
// intel must precede MS VC++, because it maskerades as MS VC++
#elif defined( __INTEL_COMPILER )
      "Intel C++ version " << to_version_string( __INTEL_COMPILER, 100 ) <<
#elif defined( _MSC_VER )
      "Microsoft Visual C++ version " << to_version_string( _MSC_VER - 600, 100 ) << " (" << _MSC_VER << ")"
#endif
      " on " << __DATE__ <<  ".\n\n" <<
      copyright <<
      "\nThis is free software; see the source for copying conditions.  There is NO"
      "\nwarranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." <<
      std::endl;

   return status;
}

// TODO (Martin#1#): expand, document
/**
 * context for read and print.
 */
struct Context
{
   Keywords keywords;      ///< the keywords specified
   WordIndex wordindex;    ///< the non-keywords collected
};

// TODO (Martin#1#): expand, document
/**
 * program's options.
 */
struct Options
{
public:
   /**
    * constructor.
    */
   Options()
   : frequency ( false )
   , ignorecase( false )
   , lowercase ( false )
   , reverse   ( false )
   , summary   ( false )
   , name_width( 20 )
   {
   }

   bool frequency;   ///< report word usage percentage and count
   bool ignorecase;  ///< (currently not used)
   bool lowercase;   ///< convert words to lowercase
   bool reverse;     ///< only report keyword (stopword) usage
   bool summary;     ///< also report number of (key)words and references

   int  name_width;  ///< name field width
};

/**
 * filename list entry exists predicate.
 */
void check_file_exists( filename_list_element_type const& element )
{
   filename_type    const filename   = element.first;
   line_number_type const linenumber = element.second;

   if ( !exist( filename ) )
   {
      if ( 0 == linenumber )
      {
         logger.Fatal( "cannot open file '" + filename + "'." );
      }
      else
      {
         logger.Fatal( "filename list, line " + to_string( linenumber ) + ": cannot open file '" + filename + "'." );
      }
   }
}

/**
 * value contained in collection predicate.
 */
template < typename C >
class contained_in : public std::unary_function< typename C::value_type, bool >
{
public:
   /**
    * the set type.
    */
   typedef C container_type;

   /**
    * the value type.
    */
   typedef typename container_type::value_type value_type;

   /**
    * the value type.
    */
   contained_in( container_type const& c )
   : m_container( c )
   {
      ;
   }

   /**
    * the value type.
    */
   bool operator()( value_type const& value ) const
   {
      return m_container.count( value ) > 0;
   }

private:
   /**
    * the set.
    */
   container_type const& m_container;
};

/**
 * read words from the given stream into the wordindex.
 */
void read( std::istream& is, Options const& options, Context& context )
{
   logger.Report( 1, "read()\n" );

   /*
    * read input tokens, add token-linenumber pairs to wordindex:
    */
   Tokenizer tokenizer( is );
   //   tokenizer.lines_per_page( opt_lines_per_page );
   tokenizer.set_lowercase( options.lowercase );

   if ( options.reverse ) // include only keywords
   {
      std::remove_copy_if
      ( tokenizer.begin(), tokenizer.end()
      , wordindex_inserter( context.wordindex )
      , std::not1( contained_in< Keywords >( context.keywords ) )
      );
   }
   else // include only non-keywords
   {
      std::remove_copy_if
      ( tokenizer.begin(), tokenizer.end()
      , wordindex_inserter( context.wordindex )
      , contained_in< Keywords >( context.keywords )
      );
   }
}

/**
 * process a file.
 */
class Reader
{
public:
   /**
    * the value type.
    */
   typedef filename_list_element_type value_type;

   /**
    * constructor.
    */
   Reader( Options const& options, Context& context )
   : m_options( options )
   , m_context( context )
   {
   }

   /**
    * read the given file.
    */
   void operator()( value_type const& element )
   {
      filename_type const filename( element.first );

      std::ifstream is( to_charptr( filename ) );

      if ( !is )
      {
         logger.Fatal( "cannot open file '" + filename + "'." );
      }

      read( is, m_options, m_context );
   }

private:
   /**
    * the options.
    */
   Options const& m_options;

   /**
    * the context (words and keywords).
    */
   Context& m_context;
};

/**
 * function object to print an entry from the colleced words.
 */
class Printer
{
   /**
    * the value type.
    */
   typedef WordIndex::value_type value_type;

public:
   /**
    * constructor.
    */
   Printer( std::ostream& os, Options const& options, Context const& context )
   : m_os( os )
   , m_options( options )
   , m_context( context )
   {
      ; // do nothing
   }

   /**
    * print the given entry.
    */
   void operator()( value_type const& value ) const
   {
      const int    count = value.second.size();
      const double perct = 100.0 * value.second.size() / m_context.wordindex.lines();

      m_os <<
         std::setw(m_options.name_width) << std::right << value.first << "  ";

      if ( m_options.frequency )
      {
         m_os <<
            std::setw(3) << std::setprecision(3) << perct << "%" << " (" << count << ")  ";
      }
      print_collection( m_os, value.second );

      m_os << std::endl;
   }

private:
   /**
    * the output stream.
    */
   std::ostream& m_os;

   /**
    * the options.
    */

   Options const& m_options;

   /**
    * the context (words and keywords).
    */
   Context const& m_context;
};

/**
 * print the collected words.
 */
void print( std::ostream& os, Options const& options, Context const& context )
{
   logger.Report( 1, "print()\n" );

   /*
    * report wordindex contents:
    */
   if ( options.summary )
   {
      os <<
         std::setw( options.name_width ) <<   "keywords" << "  " << context.keywords.size() << std::endl <<
         std::setw( options.name_width ) <<      "words" << "  " << context.wordindex.words() << std::endl <<
         std::setw( options.name_width ) << "references" << "  " << context.wordindex.lines() << std::endl << std::endl;
   }

   std::for_each
   ( context.wordindex.begin()
   , context.wordindex.end()
   , Printer( os, options, context )
   );
}

/**
 * user defined output for the tclap commandline handling.
 */
class ClapOutput : public clp::StdOutput
{
public:
   /**
    * fatal error, show help hint, terminate program.
    */
   virtual void failure( clp::CmdLineInterface& c, clp::ArgException& e )
   {
      logger.Fatal( std::string( e.what() ) +  "\n" + try_help );
   }

   /**
    * show program usage, terminate program.
    */
   virtual void usage( clp::CmdLineInterface& c )
   {
      exit( wordindex::usage( std::cout, 0 ) );
   }

   /**
    * show program version, terminate program.
    */
   virtual void version( clp::CmdLineInterface& c )
   {
      exit( wordindex::version( std::cout, 0 ) );
   }
};

/**
 * handle --author option.
 */
class AuthorVisitor : public clp::Visitor
{
private:
   std::ostream* m_output;       ///< the output stream
   std::string   m_name;         ///< the author name
public:
   /**
    * constructor.
    */
   AuthorVisitor( std::ostream* output, const std::string name )
   : m_output( output )
   , m_name  ( name   )
   {
      ; // do nothing
   }

   /**
    * handle this option (call-back).
    */
   void visit()
   {
      (*m_output) << m_name << std::endl; exit( 0 );
   }
};

/**
 * tclap commandline filename constraint(s).
 */
class FilenameConstraint : public clp::Constraint< std::string >
{
private:
   /**
    * logger to report through.
    */
   Logger& m_logger;

public:
   /**
    * constructor.
    */
   FilenameConstraint( Logger& logger )
   : m_logger( logger )
   {
      ; // do nothing
   }

   /**
    * constraint description.
    */
   virtual std::string description() const
   {
      return "no non-option arguments expected";
   }

   /**
    * constraint id.
    */
   virtual std::string shortID() const
   {
      return "filename constraint";
   }

   /**
    * true if filename does not start with - or +.
    */
   virtual bool check( const std::string& name ) const
   {
      // string-as-container approach (Kevlin Henney:  The Next Best String)

      const std::string set = "+-";

      if ( name.begin() != std::find_first_of( name.begin(), name.end(), set.begin(), set.end() ) )
//      if ( std::string::npos == value.find_first_of( "-+" ) )
      {
         return true;
      }

      logger.Fatal
         ( "unrecognized option '" + name + "' found; review the commandline.\n"
         + "Try '" + program_name + " --help' for more information.\n"
      );
      return false;
   }
};

/**
 * convert a filename to a filename list element.
 */
const filename_list_element_type
to_filename_list_element( filename_type const& filename )
{
   return filename_list_element_type( filename );
}

/**
 * the program.
 */
int main( int argc, char* argv[] )
{
   program_name = argv[0];

   /*
    * handle commandline processing exceptions:
    */
   try
   {
      /*
       * convenience using declarations and typedefs:
       */
      using   clp::SwitchArg;
      using   clp::MultiSwitchArg;
      typedef clp::ValueArg< int         > IntArg;
      typedef clp::ValueArg< double      > RealArg;
      typedef clp::ValueArg< std::string > StringArg;
      typedef clp::UnlabeledMultiArg< filename_type > FileArgs;

      /*
       * define the commanline parser and options:
       */
      clp::CmdLine cmd( "unused-message", '=', version_string );
      cmd.setOutput( new ClapOutput );

// TODO (Martin#1#): false => not-required

           SwitchArg clpAuthor    ( "a", "author"         , "", cmd, false, new AuthorVisitor( &std::cout, author_string ) );
      MultiSwitchArg clpVerbose   ( "v", "verbose"        , "", cmd, false );

           SwitchArg clpFrequency ( "f", "frequency"      , "", cmd, false );
//           SwitchArg clpIgnorecase( "g", "ignorecase"     , "", cmd, false );
           SwitchArg clpLowercase ( "l", "lowercase"      , "", cmd, false );
           SwitchArg clpReverse   ( "r", "reverse"        , "", cmd, false );
           SwitchArg clpSummary   ( "s", "summary"        , "", cmd, false );

           StringArg clpInput     ( "i", "input"          , "file with filenames", false, "[none]", "filename", cmd );
           StringArg clpOutput    ( "o", "output"         , "outut file", false, "standard output", "filename", cmd );
           StringArg clpKeywords  ( "k", "keywords"       , "keyword file", false, "[none]", "filename", cmd );

//            FileArgs fileArgs    (  "", "filenames"      , false, "type-descr.", cmd, false );
            FileArgs fileArgs    (  "", "filenames"      , false, new FilenameConstraint( logger ), cmd );

      /*
       * parse the command line:
       */
      cmd.parse( argc, argv );

      Options options;
      Context context;

      /*
       * default output file (std::cout):
       * FIXME: pointer?
       */
      std::ostream* output = &std::cout;

      /*
       * set verbosity:
       */
      logger.SetLevel( logger.GetLevel() + clpVerbose.getValue() );

      /*
       * flags:
       */
      options.frequency  = clpFrequency.isSet();
//      options.ignorecase = clpIgnorecase.isSet();
      options.lowercase  = clpLowercase.isSet();
      options.reverse    = clpReverse.isSet();
      options.summary    = clpSummary.isSet();

//      if ( options.ignorecase )
//      {
//         logger.Fatal( "option --ignorecase is not yet implemented." );
//      }

      /*
       * open output file if requested:
       */
      if ( clpOutput.isSet() )
      {
         const filename_type filename( clpKeywords.getValue() );

         output = new std::ofstream( to_charptr( filename ) );

         if ( !*output )
         {
            logger.Fatal( "cannot open file '" + filename + "' for output." );
         }
      }

      /*
       * read keywords if requested:
       */
      if ( clpKeywords.isSet() )
      {
         const filename_type filename( clpKeywords.getValue() );

         std::ifstream is( to_charptr( filename ) );

         if ( !is )
         {
            logger.Fatal( "cannot open file '" + filename + "' for output." );
         }
         else // FIXME: => read( is, options, context, ctx.keywords ); ??
         {
            Tokenizer tokenizer( is );

            std::copy
            ( tokenizer.begin(), tokenizer.end()
            , std::inserter( context.keywords, context.keywords.begin() )
            );
         }
      }

      /*
       * print banner:
       */
      logger.Report( 1, banner );

      filename_list_type filename_list;

      /*
       * copy filenames from commandline into filename list:
       */
      std::transform
      ( fileArgs.getValue().begin(), fileArgs.getValue().end()
      , std::back_inserter( filename_list )
      , to_filename_list_element
      );

      /*
       * read filenames from file if requested:
       */
      if ( clpInput.isSet() )
      {
         filename_type filename = clpInput.getValue();

         if ( !exist( filename ) )
         {
            logger.Fatal( "cannot open file '" + filename + "' to read filenames from." );
         }

         std::ifstream is( to_charptr( filename ) );

// TODO (Martin#1#): somehow combine with a read() => lowercase auto synchronizes word-keyword
         Tokenizer tokenizer( is );
         tokenizer.set_lowercase( options.lowercase );
         // FIXME: valid filename characters?
//         tokenizer.filename_scanner();
         tokenizer.add_to_followset( "-." );
         tokenizer.set_skip_comments();

         std::copy
         ( tokenizer.begin(), tokenizer.end()
         , std::back_inserter( filename_list )
         );
      }

      /*
       * check files for existence:
       */
      std::for_each( filename_list.begin(), filename_list.end(), check_file_exists );

      /*
       * process given files, or std::cin if none given:
       */
      if ( filename_list.size() <= 0 )
      {
         read( std::cin, options, context );
      }
      else
      {
         std::for_each
         ( filename_list.begin(), filename_list.end()
         , Reader( options, context )
         );
      }

      print( *output, options, context );

// TODO (Martin#1#): smart-pointer?, leaking?
      if ( output != &std::cout )
      {
         delete output;
      }
   }
   catch ( clp::ArgException& e )
   {
      logger.Fatal( e.error() + " " + e.argId() );
   }

   // FIXME: scoped; reopen()
   return 0;
}

} // namespace wordindex

/**
 * the program.
 */
int main( int argc, char* argv[] )
{
   return wordindex::main( argc, argv );
}

/*
 * end of file
 */
