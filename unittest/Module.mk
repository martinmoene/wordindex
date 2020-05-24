#
# WordIndex/Unittest/Module.mk - module rules.
#
# This file is part of WordIndex.
#
# Copyright (C) 2007-2020, Martin J. Moene.
#
# $Id: $
#

#FRUCTOSE_OPTIONS = -r
#FRUCTOSE_OPTIONS = -v


unittests: \
	unittest/Test-Fructose.exe \
	unittest/Test-Logger.exe \
	unittest/Test-Pair.exe \
	unittest/Test-Tokenizer.exe \
	unittest/Test-Utility.exe \
	unittest/Test-WordIndex.exe \
	unittest/Test-Fructose.exe  $(FRUCTOSE_OPTIONS) \
	unittest/Test-Pair.exe      $(FRUCTOSE_OPTIONS) \
	unittest/Test-Tokenizer.exe $(FRUCTOSE_OPTIONS)

#   unittest/Test-Logger.exe    $(FRUCTOSE_OPTIONS) \
#   unittest/Test-Pair.exe      $(FRUCTOSE_OPTIONS) \
#   unittest/Test-Utility.exe   $(FRUCTOSE_OPTIONS) \
#   unittest/Test-WordIndex.exe $(FRUCTOSE_OPTIONS)

unittest/Test-Fructose.exe:  unittest/Test-Fructose.cpp
unittest/Test-Logger.exe:    unittest/Test-Logger.cpp
unittest/Test-Pair.exe:      unittest/Test-Pair.cpp
unittest/Test-Utility.exe:   unittest/Test-Utility.cpp
unittest/Test-Tokenizer.exe: unittest/Test-Tokenizer.cpp
unittest/Test-WordIndex.exe: unittest/Test-WordIndex.cpp

#.cpp.exe:
#	$(CC) $(CXXFLAGS) -I include -o $*.exe $<

#
# end of file
#
