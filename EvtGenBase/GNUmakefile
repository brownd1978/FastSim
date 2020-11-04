# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile 427 2010-01-14 13:25:53Z stroili $
#################################################################
#++ library products				[build it with 'lib']

LIBREMOVEFILES := 
LIBTMPLFILES :=  
LIBDDLORDERED := 

#################################################################
#++ extra binary products	[not in production, build it with extrabin]

EXTRABINS := 

#################################################################
#++ binary products				[build it with 'bin']

BINS := 
BINCCFILES := $(BINS:=.cc) $(EXTRABINS:=.cc) 

#++ Binary rules		 [in production, build it with 'bin']

$(addprefix $(bindir),$(BINS)): $(bindir)% : %.o

#++ shell script products.. 			[build it with 'bin']
#BINSCRIPTS := testscript

#################################################################
#++ regression test scripts			[build it with 'test']


#################################################################
#++ include standard makefile from SoftRelTools.
include SoftRelTools/standard.mk
