#
# WordIndex/Doc/Module.mk - module rules.
#
# This file is part of WordIndex.
#
# Copyright (C) 2007-2020, Martin J. Moene.
#
# $Id: $
#

HLP    = doc/examples/help.txt

DOCPRG = doc/examples/copy_tokens.exe doc/examples/insert_tokens.exe
DOCOUT = doc/examples/copy_tokens.txt doc/examples/insert_tokens.txt

DOC    = doc/WordIndex.chm
DOCSRC = $(PRGSRC) $(PRGHDR) $(UNITTST) $(DOCPRG) $(DOCOUT) doc/Doxyfile doc/MainPage.txt doc/Footer.html $(HLP)

#
# Documentation:
#
$(HLP): $(PRGBIN)
	$(PRGBIN) +help >$(HLP)

doc/examples/copy_tokens.exe: doc/examples/copy_tokens.cpp
	g++ -Isrc -o doc/examples/copy_tokens.exe doc/examples/copy_tokens.cpp

doc/examples/insert_tokens.exe: doc/examples/insert_tokens.cpp
	g++ -Isrc -o doc/examples/insert_tokens.exe doc/examples/insert_tokens.cpp

doc/examples/copy_tokens.txt: doc/examples/copy_tokens.exe
	doc\examples\copy_tokens.exe < doc/examples/copy_tokens.cpp > doc/examples/copy_tokens.txt

doc/examples/insert_tokens.txt: doc/examples/insert_tokens.exe
	doc\examples\insert_tokens.exe < doc/examples/insert_tokens.cpp > doc/examples/insert_tokens.txt

$(DOC): $(DOCSRC)
	cd doc && $(DOXYGEN) && cd ..

#
# end of file
#
