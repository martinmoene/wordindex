#
# WordIndex/Makefile - create documentation, program, installer.
#
# This file is part of WordIndex.
#
# Copyright (C) 2007-2020, Martin J. Moene.
#
# $Id$
#

#
# Configuration:
#
RELEASE_NUMBER      = 0.0.4
RELEASE_DATE_NUMBER = 20200525
RELEASE_DATE_STRING = 25 May 2020

RELEASE  = $(RELEASE_NUMBER)-$(RELEASE_DATE_NUMBER)
PACKAGE  = WordIndex ($(RELEASE))
#SETUP   = Setup-wordindex-$(RELEASE).exe

MAKEFILE = Makefile

#
# Tools:
#
PATH   += C:/MinGW/bin;
#LK      = link
CP      = cp
RM      = rm
CC      = g++
CAT     = cat
SED     = sed
ECHO    = echo
FDEP    = build/fastdep
ISCC    = iscc
DOXYGEN = doxygen

# Windows/Other (OS only defined on Windows)
ifdef OS
FIND    = gfind
else
FIND    = find
endif

#
# Sources:
#
DOC     = # chm result
DOCSRC  =
PRGSRC  =
STPSRC  = $(MAKEFILE)

#
# Rules:
#
LFLAGS +=
CXXFLAGS += -Wall -Iinclude

include Makefile.rules

.SUFFIXES: .h_in

%.o: %.cpp
	$(CC) -c $(CXXFLAGS) -o$@ $<

%.exe: %.cpp
	$(CC) $(CXXFLAGS) -I include -o $@ $<

.h_in.h:
	@$(ECHO) "[\n[ Editing $< for release $(RELEASE):\n["
	$(SED) -e "s/@release_number@/$(RELEASE_NUMBER)/g" -e "s/@release_date_string@/$(RELEASE_DATE_STRING)/g" $< >$*.h

#
# Targets:
#
all: prompt unittests programs doc

prompt:
	@$(ECHO) "\n[\n[Creating package $(PACKAGE)\n["

#
# Include the description for each module:
#
include      Src/Module.mk
include      Doc/Module.mk
include unittest/Module.mk

#
# Programs:
#
programs: $(PRGBIN)

src/version.h: src/version.h_in

$(PRGVER): $(MAKEFILE) $(PRGVER)_in

#$(PRGOBJ): $(PRGHDR)

$(PRGBIN): $(PRGVER) $(PRGOBJ)   #  $(PRGRES) $(PRGLIB)
	$(CC) $(CFLAGS) -o$@ $(PRGOBJ) $(PRGLIB)

#
# Documentation:
#
doc: $(DOC) $(HLP)

#
# Installer:
#
#setup:: $(SETUP)

#
# Maintenance:
#
clean::
	-$(FIND) . -name "*.log" -o -name "*.bak" -exec $(RM) {} \;

distclean:: clean
	-$(RM) -rf doc/html
	-$(FIND) . -name "*.o" -exec $(RM) {} \;
	-$(FIND) unittest -name "*.exe" -exec $(RM) {} \;

realclean:: distclean
	-$(RM) doc/wordindex.chm src/version.h bin/*.*

#
# Dependencies:
#
dep: src/version.h Makefile.dep

#.PHONY: Makefile.dep
Makefile.dep: $(PRGVER)
	$(CC) -MM -Iinclude src/main.cpp >$@
	$(CC) -MM -Iinclude -Isrc doc/examples/copy_tokens.cpp >>$@
	$(CC) -MM -Iinclude -Isrc doc/examples/insert_tokens.cpp >>$@

include Makefile.dep

#
# End of file
#
