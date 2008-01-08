#
# WordIndex/src/Module.mk - module rules.
#
# This file is part of WordIndex.
#
# Copyright (C) 2007-2008, Martin J. Moene.
#
# $Id: $
#

PRGBIN  = bin\wordindex.exe
PRGVER  = src/version.h

PRGSRC  = $(PRGSRC) src/main.cpp

PRGHDR  = $(PRGHDR) \
          src/Config.h \
          src/Utility.h \
          src/Tokenizer.h \
          $(PRGVER)

PRGOBJ  = $(PRGOBJ) $(PRGSRC:.cpp=.o)

PRGLIB  =

#
# end of file
#

