#
# WordIndex/Unittest/Module.mk - module rules.
#
# This file is part of WordIndex.
#
# Copyright (C) 2007-2008, Martin J. Moene.
#
# $Id: $
#

#FRUCTOSE_OPTIONS = -r
#FRUCTOSE_OPTIONS = -v


unittests: \
   UnitTest/Test-Fructose.exe \
   UnitTest/Test-Logger.exe \
   UnitTest/Test-Pair.exe \
   UnitTest/Test-Tokenizer.exe \
   UnitTest/Test-Utility.exe \
   UnitTest/Test-WordIndex.exe \
   UnitTest\Test-Fructose.exe  $(FRUCTOSE_OPTIONS)
   UnitTest\Test-Pair.exe      $(FRUCTOSE_OPTIONS)
   UnitTest\Test-Tokenizer.exe $(FRUCTOSE_OPTIONS)

#   UnitTest\Test-Logger.exe    $(FRUCTOSE_OPTIONS)
#   UnitTest\Test-Pair.exe      $(FRUCTOSE_OPTIONS)
#   UnitTest\Test-Utility.exe   $(FRUCTOSE_OPTIONS)
#   UnitTest\Test-WordIndex.exe $(FRUCTOSE_OPTIONS)

UnitTest/Test-Fructose.exe:  UnitTest/Test-Fructose.cpp
UnitTest/Test-Logger.exe:    UnitTest/Test-Logger.cpp
UnitTest/Test-Pair.exe:      UnitTest/Test-Pair.cpp
UnitTest/Test-Utility.exe:   UnitTest/Test-Utility.cpp
UnitTest/Test-Tokenizer.exe: UnitTest/Test-Tokenizer.cpp
UnitTest/Test-WordIndex.exe: UnitTest/Test-WordIndex.cpp

.cpp.exe:
   $(CC) $(CXXFLAGS) -I include -o $*.exe $<

#
# end of file
#
