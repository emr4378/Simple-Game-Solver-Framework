#
# Created by gmakemake (Ubuntu Sep 14 2011) on Tue Nov  8 18:26:16 2011
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Default flags (redefine these with a header.mak file if desired)
CXXFLAGS =	-ggdb
CFLAGS =	-ggdb
CLIBFLAGS =	-lm
CCLIBFLAGS =	
########## End of default flags


CPP_FILES =	crossout.cpp nim.cpp othello.cpp takeaway.cpp
C_FILES =	
PS_FILES =	
S_FILES =	
H_FILES =	crossout.h evaluator.h game.h nim.h othello.h takeaway.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	

#
# Main targets
#

all:	crossout nim othello takeaway 

crossout:	crossout.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o crossout crossout.o $(OBJFILES) $(CCLIBFLAGS)

nim:	nim.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o nim nim.o $(OBJFILES) $(CCLIBFLAGS)

othello:	othello.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o othello othello.o $(OBJFILES) $(CCLIBFLAGS)

takeaway:	takeaway.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o takeaway takeaway.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

crossout.o:	crossout.h game.h
nim.o:	evaluator.h game.h nim.h
othello.o:	evaluator.h game.h othello.h
takeaway.o:	evaluator.h game.h takeaway.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) crossout.o nim.o othello.o takeaway.o core

realclean:        clean
	-/bin/rm -f crossout nim othello takeaway 
