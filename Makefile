#
# WordIndex/Makefile - create documentation, program, installer.
#
# This file is part of WordIndex.
#
# Copyright (C) 2007-2008, Martin J. Moene.
#
# $Id$
#

#
# Configuration:
#
RELEASE_NUMBER      = 0.0.3
RELEASE_DATE_NUMBER = 20191030
RELEASE_DATE_STRING = 30 Oct 2019

RELEASE  = $(RELEASE_NUMBER)-$(RELEASE_DATE_NUMBER)
PACKAGE  = WordIndex ($(RELEASE))
#SETUP   = Setup-wordindex-$(RELEASE).exe

#
# Tools:
#
PATH    = C:/Programs/MinGW/bin;C:/Programs/msys/1.0/bin;$(PATH);
#LK      = link
CP      = cp
RM      = rm
CC      = g++
CAT     = cat
SED     = build\sed
FDEP    = build\fastdep
FIND    = find
ISCC    = iscc
DOXYGEN = doxygen

#
# Sources:
#
DOC     = # chm result
DOCSRC  =
PRGSRC  =
STPSRC  = Makefile

#
# Rules:
#
#RFLAGS = $(RFLAGS) -fo"$(PRGRES)" -d "NDEBUG"
#CFLAGS = $(CFLAGS) -nologo -W3 -EHsc -MT -Iinclude -Isrc -Ivendor
CFLAGS = $(CFLAGS) -Wall -Iinclude -Isrc # -Ivendor
LFLAGS = $(LFLAGS)

#CXXFLAGS = $(CXXFLAGS) -nologo -W4 -GX -Iinclude # -Ivendor
CXXFLAGS = $(CXXFLAGS) -Wall -Iinclude # -Ivendor

!include Makefile.rules

.SUFFIXES: .h_in

#.c.o:
#   $(CC) -c $(CFLAGS) -o$*.o $<

{src}.cpp{src}.o:
   $(CC) -c $(CXXFLAGS) -o$*.o $<

#{src}.cpp{o}.o:
#   $(CC) -c $(CXXFLAGS) -o$*.o $<

{src}.h_in{src}.h:
   @echo.
   @echo Editing $< for release $(RELEASE):
   $(SED) -e "s/@release_number@/$(RELEASE_NUMBER)/g" -e "s/@release_date_string@/$(RELEASE_DATE_STRING)/g" $< >$*.h

#
# Targets:
#
all: prompt unittests programs doc

prompt:
   @$(CAT) <<
[
[ Creating package $(PACKAGE)
[

<<

#
# Include the description for each module:
#
!include      Src/Module.mk
!include      Doc/Module.mk
!include UnitTest/Module.mk

#
# Programs:
#
programs: $(PRGBIN)

src/version.h: src/version.h_in

$(PRGVER): Makefile $(PRGVER)_in

#$(PRGOBJ): $(PRGHDR)

$(PRGBIN): $(PRGOBJ)   #  $(PRGRES) $(PRGLIB)
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
   -$(FIND) . -name *.log -o -name *.bak -exec $(RM) {} ;

distclean:: clean
   -$(RM) -rf doc/html 2>nul
   -$(FIND) . -name *.obj -exec $(RM) {} ;
   -$(FIND) . -name *.o -exec $(RM) {} ;
   -$(FIND) unittest -name *.exe -exec $(RM) {} ;

realclean:: distclean
   -$(RM) doc/*.chm src/version.h bin/*.* 2>nul

#
# Dependencies:
#
dep: src/version.h Makefile.dep

Makefile.dep: FORCE
   $(CC) -MM -Iinclude src/main.cpp >$@
   $(CC) -MM -Iinclude;../../src doc/examples/copy_tokens.cpp >>$@
   $(CC) -MM -Iinclude;../../src doc/examples/insert_tokens.cpp >>$@

# edit main.o => src/main.o   $@.tmp

!include Makefile.dep

#
# Other:
#
FORCE:

#
# End of file
#
