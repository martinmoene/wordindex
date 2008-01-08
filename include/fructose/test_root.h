/* FRUCTOSE C++ unit test library. 
Copyright (c) 2007 Andrew Peter Marlow. All rights reserved.
http://www.andrewpetermarlow.co.uk.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef INCLUDED_FRUCTOSE_TEST_ROOT
#define INCLUDED_FRUCTOSE_TEST_ROOT

#include "fructose/double_compare.h"
#include "tclap/CmdLine.h"

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include <cstdlib>

// still the best for floating point output IMO.
#include <stdio.h>

#if defined(WIN32) || defined(_WIN32)
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

/**
 * All unit test facilities associated are scoped to the fructose namespace.
 */
namespace fructose {
   
/**
 * Base class for test_base template.
 * 
 * This class keeps track of the test mode flags and
 * of the error count. It is responsible for formatting
 * the test failure messages. It also takes care of the
 * dynamic initialisation of test suites from the command
 * line.
 */
class test_root
{
public:

    /**
     * Destructor
     * 
     * Virtual in order to enable dynamic_cast
     */
    virtual ~test_root();

    /**
     * Default constructor
     * 
     * Initialise error counter and mode flags.
     */   
    test_root();
         
    /**
     * Accessor for verbose mode flag 
     * 
     * Return true if verbose mode is on.
     * 
     * @return The current setting of the verbose mode flag
     */
    bool verbose() const;
    
    /**
     * Setter for verbose mode flag 
     * 
     * Switch on/off verbose mode.
     * 
     * @param flag - the desired setting of the verbose mode flag
     */
    void verbose(bool flag);
    
    /**
     * Accessor for stop-on-failure mode flag
     * 
     * Return true if stop-on-failure mode is set.
     * By default this value is false, i.e. when a test fails
     * the harness continues to run the other tests, producing a count
     * of all the failed tests at the end.
     * 
     * @return The current setting of the stop-on-failure mode flag
     */
    bool stop_on_failure() const;
    
    /**
     * Setter for stop-on-failure mode flag
     * 
     * Switch on/off stop-on-failure mode.
     * By default this flag is off, Turning it on causes the
     * test harness to consider the first assertion failure to
     * be fatal. 
     * 
     * @param flag - the desired setting of the stop-on-failure mode
     */
    void stop_on_failure(bool flag);
    
    /**
     * Accessor for reverse mode flag
     * 
     * Return true if reverse mode is set. Nornally this will never
     * be true, it is provided for testing the framework.
     * It is used by the -reverse command line option for such testing,
     * to reverse the sense of test assertions.
     * 
     * @return The current setting of the reverse mode flag
     */
    bool reverse_mode() const;
    
    /**
     * Setter for reverse mode flag
     * 
     * Revert the meaning of condition for convenient
     * testing of the test framework.
     * 
     * @param flag - the desired setting of the reverse mode
     */
    void reverse_mode(bool flag);
    
    /**
     * Accessor for error status
     * 
     * Return true if an assertion failed.
     * 
     * @return The current error status - true if there was a failed assertion
     */
    bool error() const;
     
    /**
     * Accessor for number of failed assertions
     * 
     * Return number of failed assertions.
     * 
     * @return The current number of failed assertions
     */
    int error_count() const;
    
    /**
     * Accessor for test return status
     * 
     * Compute the return status for use by test driver main.
     * 
     * @return Return status: EXIT_SUCCESS if error count is 0, 
     * EXIT_FAILURE otherise
     */
    int return_status() const;

    /**
     * Setup initial conditions for each test. 
     */
    virtual void setup();

    /**
     * Undo any work that needs undoing from setup (for each test).
     */
    virtual void teardown();

    /**
     * Set the name of the current test.
     */
    void set_test_name(const std::string& test_name);

    /**
     * Return the name of the current test.
     */
    std::string get_test_name() const;
    
    /**
     * Tests the specified condition which must be true for the test to pass.
     * If the condition is false then test case name, the filename, and line 
     * number are reported and the error count is increased. Note that because 
     * this sets an error status the method cannot be static.
     * 
     * @param condition - the condition to be checked by the test
     * @param test_case_name - the name of the enclosing test case
     * @param msg - the message to be printed on failure
     * @param filename - the name of the file containing the test
     * @param line_number - the location of the test assertion in the test source 
     * file
     */
    void test_assert(bool condition, const std::string& test_case_name, 
                     const char *msg, const char* filename, int line_number);

    /**
     * Tests that two values are equal.
     * If this condition is false then test case name, the filename, and line 
     * number are reported and the error count is increased. Note that because 
     * this sets an error status the method cannot be static.
     * 
     * @param lhs - the value on the left hand side of the expression (lhs == rhs)
     * @param rhs - the value on the right hand side of the expression (lhs == rhs)
     * @param test_case_name - the name of the enclosing test case
     * @param msg - the message to be printed on failure
     * @param filename - the name of the file containing the test
     * @param line_number - the location of the test assertion in the test source 
     * file
     */
    template <typename lhs_type, typename rhs_type>
    void test_assert_eq(const lhs_type& lhs, const rhs_type& rhs,
                            const std::string& test_case_name, 
                            const char *msg1, const char* msg2,
                            const char* filename, int line_number);

    /**
     * Tests that two doubles are equal.
     * The floating point comparison is made using default values
     * for relative and absolute tolerance.
     * If this condition is false then test case name, the filename, and line 
     * number are reported and the error count is increased. Note that because 
     * this sets an error status the method cannot be static.
     * 
     * @param lhs - the value on the left hand side of the expression (lhs == rhs)
     * @param rhs - the value on the right hand side of the expression (lhs == rhs)
     * @param test_case_name - the name of the enclosing test case
     * @param msg - the message to be printed on failure
     * @param filename - the name of the file containing the test
     * @param line_number - the location of the test assertion in the test source 
     * file
     */
    void test_assert_double_eq(double lhs, double rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * Tests that two doubles are not equal.
     */
    void test_assert_double_ne(double lhs, double rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs < rhs.
     */
    void test_assert_double_lt(double lhs, double rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs <= rhs.
     */
    void test_assert_double_le(double lhs, double rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs > rhs.
     */
    void test_assert_double_gt(double lhs, double rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs >= rhs.
     */
    void test_assert_double_ge(double lhs, double rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * Tests that two doubles are equal.
     * The floating point comparison is made with specified values
     * for relative and absolute tolerance.
     * If this condition is false then test case name, the filename, and line 
     * number are reported and the error count is increased. Note that because 
     * this sets an error status the method cannot be static.
     * 
     * @param lhs - the value on the left hand side of the expression (lhs == rhs)
     * @param rhs - the value on the right hand side of the expression (lhs == rhs)
     * @param test_case_name - the name of the enclosing test case
     * @param msg - the message to be printed on failure
     * @param filename - the name of the file containing the test
     * @param line_number - the location of the test assertion in the test source 
     * file
     */
    void test_assert_double_eq(double lhs, double rhs,
                   double relative_tolerance, double absolute_tolerance,
                            const std::string& test_case_name, 
                            const char *msg1, const char* msg2,
                            const char* filename, int line_number);

    /**
     * Tests that two doubles are not equal.
     */
    void test_assert_double_ne(double lhs, double rhs,
                   double relative_tolerance, double absolute_tolerance,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs < rhs.
     */
    void test_assert_double_lt(double lhs, double rhs,
                   double relative_tolerance, double absolute_tolerance,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs <= rhs.
     */
    void test_assert_double_le(double lhs, double rhs,
                   double relative_tolerance, double absolute_tolerance,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs > rhs.
     */
    void test_assert_double_gt(double lhs, double rhs,
                   double relative_tolerance, double absolute_tolerance,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * tests that lhs >= rhs.
     */
    void test_assert_double_ge(double lhs, double rhs,
                   double relative_tolerance, double absolute_tolerance,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number);

    /**
     * Type of test suite
     * 
     * A suite is just a list of test case names.
     */
    typedef std::vector<std::string> suite;
    
    /**
     * Generic execption handler for use in test drivers
     * 
     * Rethrow current exception, print what string for standard exceptions
     * and "unknown exception" for any other exception. Return EXIT_FAILURE
     * for use as exit code by the main function of the test driver. 
     * 
     * @return EXIT_FAILURE
     */    
    static int exception_handler();

    /**
     * Convenience function for underlying a title
     * 
     * Create a string of a specifyable underline character for underlying 
     * a given title. This is used when the verbose flag is on, to show
     * in a pretty way which test is being run.
     * 
     * @param title - The title to be underlined
     * @param uline - The character to be used for underlining
     * 
     * @return The underlining string
     */
    static std::string underline(const std::string& title, char uline = '=');

    /**
     * Decrease error count.
     * It is possible to have tests involving exception handling that
     * cause std::uncaught_exception to be true, even if no exception 
     * escapes.
     * These routines can call this method to fix the error count.
     */    
    void decrease_error_count();

protected:

     /**
      * 
      * Increase error count by one.
      */
    void increase_error_count();
    
    /**
     * Populate test suite.
     * 
     * Populate suite from command line, taking into account 
     * the test case available_tests.
     * Display usage info and exit if -h flag set.
     * 
     * @param available_tests - The list of available tests
     * @param argc     - The argument counter parameter passed to main
     * @param argv     - The argument array passed to main
     * 
     * @return The list of test cases selected by the user
     */
    suite do_get_suite(const suite& available_tests, int argc, char* argv[]);
    
private:

    /**
     * The verbose mode flag
     * verbose mode flag
     */
    bool m_verbose;

    /**
     * The stop-on-failure flag
     */
    bool m_stop_on_failure;

    /**
     * The reverse mode flag
     */
    bool m_reverse_mode;

    /**
     * The error count
     */    
    int m_error_count;

    /**
     * The name of the current test.
     */    
    std::string m_test_name;

    // disable copying
    test_root(const test_root&);
    test_root& operator=(const test_root&);
};

// ====================
// INLINE definitions
// ====================

inline
test_root::test_root() 
    : m_verbose(false) 
    , m_stop_on_failure(false)
    , m_reverse_mode(false)
    , m_error_count(0)
{
}

inline
test_root::~test_root()
{
    if (std::uncaught_exception())
    {
        increase_error_count();
    }
        
    if (error_count() > 0)
    {
        std::cout
            << "\nTest driver failed: " 
            << error_count() << (error_count() == 1 ? " error" : " errors") 
            << std::endl;
    }
    else if (verbose())
    {
        std::cout << "\nTest driver succeeded." << std::endl;
    }
}

inline
bool test_root::verbose() const
{
    return m_verbose;    
}

inline
void test_root::verbose(bool flag) 
{
    m_verbose = flag;
}

inline
bool test_root::stop_on_failure() const
{
    return m_stop_on_failure;    
}

inline
void test_root::stop_on_failure(bool flag) 
{
    m_stop_on_failure = flag;
}

inline
bool test_root::reverse_mode() const
{
    return m_reverse_mode;    
}

inline
void test_root::reverse_mode(bool flag) 
{
    m_reverse_mode = flag;
}

inline
int test_root::error_count() const 
{
    return m_error_count;
}

inline
bool test_root::error() const
{
    return m_error_count > 0;    
}

inline
int test_root::return_status() const
{
    return m_error_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;    
}

inline
void test_root::setup()
{
}

inline
void test_root::teardown()
{
}

inline
void test_root::set_test_name(const std::string& test_name)
{
    m_test_name = test_name;
}

inline
std::string test_root::get_test_name() const
{
    return m_test_name;
}

inline
void
test_root::increase_error_count()
{
    ++m_error_count;
}

inline
void
test_root::decrease_error_count()
{
    if (m_error_count > 0)
    {
         --m_error_count;
    }
}

inline
void test_root::test_assert(
    bool condition, const std::string& test_case_name, 
    const char *msg, const char* filename, int line_number)
{
    if (condition == reverse_mode())
    {
        std::cout
            << "Error: " << test_case_name << " in " 
            << filename << "(" << line_number << "): " 
            << msg << " failed." << std::endl;
                
        increase_error_count();
        
        if (stop_on_failure())
        {
            // decrement error count to compensate for later increase
            // caused by uncaught exception bumping the count in
            // test_root::exception_handler.

            decrease_error_count();
            throw std::runtime_error("test assertion failed");
        }
    }
}

inline
test_root::suite 
test_root::do_get_suite(const test_root::suite& available_tests, 
                      int argc, char* argv[])
{
    TCLAP::CmdLine cmd("-verbose turns on extra trace for those "
                       "tests that have made use of it.\n"
                       "-assert_fatal is used to make the first "
                       "test failure fatal.\n"
                       "-help produces this help.\n"
                       "-reverse reverses the sense of test assertions.\n"
                       "It is  only used to test the test framework itself.\n"
                       "Any number of test names may be supplied on the "
                       "command line. "
                       "If the name 'all' is included then all tests will "
                       "be run."
                       "", ' ', "1.0", false); 
    TCLAP::SwitchArg verbose_arg("v", "verbose", "-v[erbose]", false); 
    TCLAP::SwitchArg assert_fatal_arg("a", "assert_fatal", "-a[ssert_fatal]", false); 
    TCLAP::SwitchArg reverse_arg("r", "reverse", "-r[everse]", false); 
    TCLAP::SwitchArg help_arg("h", "help", "-h[elp]", false); 
    TCLAP::UnlabeledMultiArg<std::string> test_names("testNames", "test names", false, "testNameString", cmd);

    cmd.add(verbose_arg);
    cmd.add(assert_fatal_arg);
    cmd.add(reverse_arg);
    cmd.add(help_arg);
    cmd.parse(argc, argv);

    verbose(verbose_arg.getValue());
    stop_on_failure(assert_fatal_arg.getValue());
    reverse_mode(reverse_arg.getValue());
    suite result = test_names.getValue();

    const bool includeAll =  
        std::find(result.begin(), result.end(), "all") != result.end();

    if (result.size() == 0 || includeAll)
    {
        result = available_tests;
    }

    if (help_arg.getValue())
    {
        cmd.getOutput()->usage(cmd);
        std::cout << "Supported test names are:" << std::endl;
        for (suite::const_iterator it = available_tests.begin(); 
             it != available_tests.end(); ++it)
        {
            std::cout << "    " << *it << std::endl;
        }
        exit(EXIT_SUCCESS);
    }

    return result;
}

inline
int test_root::exception_handler()
{
    try
    {
        throw;
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;        
    }
    catch (...)
    {
        std::cout << "Error: " << "unknown exception" << std::endl;
    }    
    return EXIT_FAILURE;
}

inline
std::string test_root::underline(const std::string& title, char c)
{
    std::string result;
    for (size_t i = 0; i != title.size(); ++i)
        result += c;        
    return result;
}

// The fructose double comparison routines are the same apart from
// the name of the double_compare function actually called and
// the string ("==", "!=" etc) in the diagnostic message.
// Hence we generate the inline function definition using the
// macros below. Note that we have two macros; one for the comparison
// with specific tolerances stated, the other without.

#define generate_fructose_assert_double_compare(comparison_function, \
                                                comparison_name) \
inline \
void test_root::test_assert_double_##comparison_function(double lhs, \
                                   double rhs, \
                                   const std::string& test_case_name, \
                                   const char* lhs_name, \
                                   const char* rhs_name,\
                                   const char* filename, int line_number) \
{ \
    bool is_compared = double_compare::comparison_function(lhs, rhs);\
    if (is_compared == reverse_mode()) \
    { \
        char number1[22]; \
        char number2[22]; \
        snprintf(number1, sizeof(number1), "%21.15e", lhs); \
        snprintf(number2, sizeof(number2), "%21.15e", rhs); \
        std::cout \
            << "Error: " << test_case_name << " in "  \
            << filename << "(" << line_number << "): " \
            << lhs_name << " " comparison_name " " << rhs_name \
            << " (" << number1 << " " comparison_name " " << number2 << ")" \
            << " failed floating point compare." << std::endl; \
        increase_error_count(); \
        if (stop_on_failure()) \
        { \
            decrease_error_count(); \
            throw std::runtime_error("test assertion failed"); \
        } \
    } \
}

#define generate_fructose_assert_double_compare_tol(comparison_function, \
                                                comparison_name) \
inline \
void test_root::test_assert_double_##comparison_function(double lhs, \
                                   double rhs, \
                                   double relative_tolerance, \
                                   double absolute_tolerance, \
                                   const std::string& test_case_name, \
                                   const char* lhs_name, \
                                   const char* rhs_name,\
                                   const char* filename, int line_number) \
{ \
    bool is_compared = double_compare::comparison_function(lhs, rhs, \
                            relative_tolerance, absolute_tolerance); \
    if (is_compared == reverse_mode()) \
    { \
        char number1[22]; \
        char number2[22]; \
        snprintf(number1, sizeof(number1), "%21.15e", lhs); \
        snprintf(number2, sizeof(number2), "%21.15e", rhs); \
        std::cout \
            << "Error: " << test_case_name << " in "  \
            << filename << "(" << line_number << "): " \
            << lhs_name << " " comparison_name " " << rhs_name \
            << " (" << number1 << " " comparison_name " " << number2 << ")" \
            << " failed floating point compare." << std::endl; \
        increase_error_count(); \
        if (stop_on_failure()) \
        { \
            decrease_error_count(); \
            throw std::runtime_error("test assertion failed"); \
        } \
    } \
}

// Generate the code for all the inline comparison function definitions.

generate_fructose_assert_double_compare(eq, "==")
generate_fructose_assert_double_compare(ne, "!=")
generate_fructose_assert_double_compare(lt, "<")
generate_fructose_assert_double_compare(le, "<=")
generate_fructose_assert_double_compare(gt, ">")
generate_fructose_assert_double_compare(ge, ">=")

// same again but this time with the tolerances specified.

generate_fructose_assert_double_compare_tol(eq, "==")
generate_fructose_assert_double_compare_tol(ne, "!=")
generate_fructose_assert_double_compare_tol(lt, "<")
generate_fructose_assert_double_compare_tol(le, "<=")
generate_fructose_assert_double_compare_tol(gt, ">")
generate_fructose_assert_double_compare_tol(ge, ">=")

template <typename lhs_type, typename rhs_type>
inline
void test_root::test_assert_eq(const lhs_type& lhs, const rhs_type& rhs,
                                   const std::string& test_case_name, 
                                   const char *msg1, const char* msg2,
                                   const char* filename, int line_number)
{
    if ((lhs == rhs) == reverse_mode())
    {
        std::cout
            << "Error: " << test_case_name << " in " 
            << filename << "(" << line_number << "): " 
            << msg1 << " == " << msg2
            << " (" << lhs << " == " << rhs << ")"
            << " failed." << std::endl;
                
        increase_error_count();
        if (stop_on_failure())
        {
            // decrement error count to compensate for later increase
            // caused by uncaught exception bumping the count in
            // test_root::exception_handler.

            decrease_error_count();
            throw std::runtime_error("test assertion failed");
        }
    }
}

} // namespaces

#define fructose_assert(X) { fructose::test_root::test_assert((X), \
                             get_test_name(),  #X, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT(X) fructose_assert(X)

#define fructose_assert_eq(X,Y) { \
           fructose::test_root::test_assert_eq((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_EQ(X,Y) fructose_assert_eq(X,Y)

// =========================
// floating point assertions
// =========================

#define fructose_assert_double_eq(X,Y) { \
           fructose::test_root::test_assert_double_eq((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_EQ(X,Y) fructose_assert_double_eq(X,Y)

#define fructose_assert_double_ne(X,Y) { \
           fructose::test_root::test_assert_double_ne((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_NE(X,Y) fructose_assert_double_ne(X,Y)

#define fructose_assert_double_lt(X,Y) { \
           fructose::test_root::test_assert_double_lt((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_LT(X,Y) fructose_assert_double_lt(X,Y) 

#define fructose_assert_double_le(X,Y) { \
           fructose::test_root::test_assert_double_le((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_LE(X,Y) fructose_assert_double_le(X,Y)

#define fructose_assert_double_gt(X,Y) { \
           fructose::test_root::test_assert_double_gt((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_GT(X,Y) fructose_assert_double_gt(X,Y)

#define fructose_assert_double_ge(X,Y) { \
           fructose::test_root::test_assert_double_ge((X), (Y), \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_GE(X,Y) fructose_assert_double_ge(X,Y)

// same again but with tolerances

#define fructose_assert_double_eq_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance){\
           fructose::test_root::test_assert_double_eq((X), (Y), \
                 relative_tolerance, absolute_tolerance, \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_EQ_REL_ABS(X,Y, \
           relative_tolerance, absolute_tolerance) \
    fructose_assert_double_eq_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance)

#define fructose_assert_double_ne_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance){\
           fructose::test_root::test_assert_double_ne((X), (Y), \
                 relative_tolerance, absolute_tolerance, \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_NE_REL_ABS(X,Y, \
           relative_tolerance, absolute_tolerance) \
    fructose_assert_double_ne_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance)

#define fructose_assert_double_lt_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance){\
           fructose::test_root::test_assert_double_lt((X), (Y), \
                 relative_tolerance, absolute_tolerance, \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_LT_REL_ABS(X,Y, \
           relative_tolerance, absolute_tolerance) \
    fructose_assert_double_lt_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance)

#define fructose_assert_double_le_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance){\
           fructose::test_root::test_assert_double_le((X), (Y), \
                 relative_tolerance, absolute_tolerance, \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_LE_REL_ABS(X,Y, \
           relative_tolerance, absolute_tolerance) \
    fructose_assert_double_le_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance)

#define fructose_assert_double_gt_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance){\
           fructose::test_root::test_assert_double_gt((X), (Y), \
                 relative_tolerance, absolute_tolerance, \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_GT_REL_ABS(X,Y, \
           relative_tolerance, absolute_tolerance) \
    fructose_assert_double_gt_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance)

#define fructose_assert_double_ge_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance){\
           fructose::test_root::test_assert_double_ge((X), (Y), \
                 relative_tolerance, absolute_tolerance, \
           get_test_name(),  #X, #Y, __FILE__, __LINE__);}

#define FRUCTOSE_ASSERT_DOUBLE_GE_REL_ABS(X,Y, \
           relative_tolerance, absolute_tolerance) \
    fructose_assert_double_ge_rel_abs(X,Y, \
           relative_tolerance, absolute_tolerance)

// ====================
// loop assertions
// ====================

#define fructose_loop_assert(I,X) { \
    if ((X) == reverse_mode()) \
    { std::cout << #I << ": " << I << "\n"; \
                       fructose::test_root::test_assert(reverse_mode(), \
                       get_test_name(),  #X, __FILE__, __LINE__); }}

#define FRUCTOSE_LOOP_ASSERT(I,X) fructose_loop_assert(I,X)

#define fructose_loop1_assert(LN,I,X) { \
    if ((X) == reverse_mode()) \
    { std::cout << #LN << ": " << LN << "\n" \
            << "index " << #I << " = " << I << "\n"; \
            fructose::test_root::test_assert(reverse_mode(), \
             get_test_name(),  #X,  \
             __FILE__, __LINE__); } }

#define FRUCTOSE_LOOP1_ASSERT(LN,I,X) fructose_loop1_assert(LN,I,X)

#define fructose_loop2_assert(LN,I,J,X) { \
    if ((X) == reverse_mode()) \
    { std::cout << #LN << ": " << LN << "\n" \
            << "index " << #I << " = " << I << "\n"; \
            << "index " << #J << " = " << J << "\n"; \
            fructose::test_root::test_assert(reverse_mode(), \
            get_test_name(),  #X,  __FILE__, __LINE__); } }

#define FRUCTOSE_LOOP2_ASSERT(LN,I,J,X) fructose_loop2_assert(LN,I,J,X)

#define fructose_loop3_assert(LN,I,J,K,X) { \
    if ((X) == reverse_mode()) \
        { std::cout << #LN << ": " << LN << "\n" \
            << "index " << #I << " = " << I << "\n"; \
            << "index " << #J << " = " << J << "\n"; \
            << "index " << #K << " = " << K << "\n"; \
            fructose::test_root::test_assert(reverse_mode(), \
            get_test_name(),  #X,  __FILE__, __LINE__); } }

#define FRUCTOSE_LOOP3_ASSERT(LN,I,J,K,X) fructose_loop3_assert(LN,I,J,K,X)

#define fructose_loop4_assert(LN,I,J,K,L,X) { \
    if ((X) == reverse_mode()) \
    { std::cout << #LN << ": " << LN << "\n" \
            << "index " << #I << " = " << I << "\n"; \
            << "index " << #J << " = " << J << "\n"; \
            << "index " << #K << " = " << K << "\n"; \
            << "index " << #L << " = " << L << "\n"; \
            fructose::test_root::test_assert(reverse_mode(), \
                get_test_name(),  #X, __FILE__, __LINE__); } }

#define FRUCTOSE_LOOP4_ASSERT(LN,I,J,K,L,X) fructose_loop4_assert(LN,I,J,K,L,X)

#define fructose_loop5_assert(LN,I,J,K,L,M,X) { \
    if ((X) == reverse_mode()) \
        { std::cout << #LN << ": " << LN << "\n" \
            << "index " << #I << " = " << I << "\n"; \
            << "index " << #J << " = " << J << "\n"; \
            << "index " << #K << " = " << K << "\n"; \
            << "index " << #L << " = " << L << "\n"; \
            << "index " << #M << " = " << M << "\n"; \
            fructose::test_root::test_assert(reverse_mode(), get_test_name(),  #X, \
                                             __FILE__, __LINE__); } }

#define FRUCTOSE_LOOP5_ASSERT(LN,I,J,K,L,M,X) fructose_loop5_assert(LN,I,J,K,L,M,X)

#define fructose_assert_exception(X, E) \
    {                                \
        std::string report;          \
        bool ok = false;             \
        bool caught = false;         \
        try                          \
        {                            \
            X;                       \
        }                            \
        catch(E&)                    \
        {                            \
            ok = true;               \
            caught = true;           \
        }                            \
        catch(std::exception & e)    \
        {                            \
            std::ostringstream ostr; \
            ostr << e.what();        \
            report = "exception caught as std::exception: "; \
            report += ostr.str();     \
            caught = true;           \
        }                            \
        catch(...)                   \
        {                            \
            caught = true;           \
            report = "caught in catchall"; \
        }                            \
        if (!ok)             \
        {                    \
            if (caught)      \
            {                \
   std::cout << "exception caught but not of expected type" << std::endl; \
   if (report.length() > 0) \
   { \
       std::cout <<  report << std::endl; \
   } \
            }                \
            else             \
            {                \
   std::cout << "expected an exception to be thrown but catchall caught nothing" << std::endl; \
            }                \
        }                    \
        fructose::test_root::test_assert(ok, get_test_name(),  #X " throws " #E, __FILE__, __LINE__);} 

#define FRUCTOSE_ASSERT_EXCEPTION(X, E) fructose_assert_exception(X, E)

#define fructose_assert_no_exception(X) {      \
        bool no_exception_thrown = true; \
        try {X;}                         \
        catch(std::exception& e)         \
        {                                \
            no_exception_thrown = false; \
std::cout << "Error: An exception was thrown where none expected. " \
<< "Exception is: " << e.what() << std::endl; \
            fructose_assert(no_exception_thrown)  \
        }                                \
        catch(...)                       \
        {                                \
            no_exception_thrown = false; \
            fructose_assert(no_exception_thrown)  \
        }}

#define FRUCTOSE_ASSERT_NO_EXCEPTION(X) fructose_assert_no_exception(X)

#endif
