/* FRUCTOSE C++ unit test library. 
Copyright (c) 2007 Chris Main and Andrew Peter Marlow. All rights reserved.
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

#include "fructose/generator.h"

#include <stdlib.h>

/*
 * This is a simple code generator that parses a source file
 * which is the test class. When FRUCTOSE is used without the
 * code generator, the test class file normally contains
 * function main at the end, with code to add the relevant
 * tests to the test suite. However, this is tedious and
 * error-prone; it is easy to miss tests when adding to 
 * the suite. This is where the code generator comes in.
 * Run the code generator and it will produce on stdout
 * the code for function main. In order for this to work
 * the test class must use certain macros:-
 *
 * Use FRUCTOSE_CLASS(className) to declare the test class. 
 *
 * Use FRUCTOSE_TEST(testName) to declare the test function
 * within the test class.
 *
 * In the examples directory, see ex6, which shows the generator in use.
*/
int main(int argc, char *argv[])
{
      fructose::generate_test_harness(argc, argv);
      return EXIT_SUCCESS;
}
