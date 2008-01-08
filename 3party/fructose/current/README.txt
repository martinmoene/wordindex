FRUCTOSE - FRamework for Unit testing C++ for Test driven development Of SoftwarE

Version 0.6.0

This is a simple unit test framework to facilitate writing standalone
programs, one per class implementation file, that allow the developer
to evolve the class using Test Driven Development (TDD).
The library is implemented completely in header files.
Look to the examples to see how it is used. Doxygen output is also provided.

John Appleby-Alis spotted a bug in the loop assert macros where they were
not using the reverse_mode function to test the condition. This meant
that the reverse flag did not work with these assertions. This has been
fixed. Thank's John for finding that one!

Any and all feedback is welcome to: andrew@andrewpetermarlow.co.uk

