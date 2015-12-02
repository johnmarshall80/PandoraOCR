# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/lib/image
# Filename:		makefile.mak
# Integrator:		Michael D. Garris
# Organization:		NIST/NCSL/875.12
# Host System:		SOLARIS SUN OS/5.2 (UNIX)
# Date Created:		6/10/94
#
# **********************************************************
# Makefile for SRC Library Directory
# **********************************************************
SHELL=/bin/sh
# PROJDIR and INSTARCH Passed On Command Line ONLY
PROJECT_ROOT	= $(PROJDIR)
ARCHTYPE	= $(INSTARCH)
LIBDIR		= $(PROJECT_ROOT)/lib
SRCDIR		= $(PROJECT_ROOT)/src/lib/image

# **************************************************
# INDIVIDUAL PROGRAM MODIFICATIONS SHOULD BEGIN HERE
# **************************************************

# Library to make (library name)
LIBRARY	= libimage.a
# Source files for LIBRARY
SRC	= bincopy.c \
	binfill.c \
	binlogop.c \
	binor.c \
	bit2byte.c \
	bitmasks.c \
	byte2bit.c \
	char2bin.c \
	chrpixel.c \
	corners.c \
	fitimage.c \
	grp4comp.c \
	grp4deco.c \
	histgram.c \
	imageops.c \
	imagutil.c \
	masks.c \
	morphchr.c \
	readrast.c \
	thresh.c \
	writrast.c \
	zoom.c

# Local additions for CFLAG options (eg: -g)
LOCAL_CFLAGS	= -xO3

# *******************************************************
# THE REST OF THE MAKEFILE SHOULD NOT NEED TO BE MODIFIED
#       (EXCEPT UPON APPROVAL OF PROJECT MANAGER)
# *******************************************************

INCLUDE = $(PROJECT_ROOT)/include
OBJ     = $(SRC:.c=.o)
LIBRY   = $(LIBRARY)

CFLAGS	= -I$(INCLUDE) -L$(LIBDIR) -s $(LOCAL_CFLAGS)
CC	= cc $(CFLAGS)

ARFLAGS = ru
AR	= ar $(ARFLAGS)

MAKEFILE	= makefile.mak
.PRECIOUS: $(MAKEFILE)

.c.o:
	$(CC) -c $<

it: $(LIBRY)

install: $(MAKEFILE) $(LIBDIR)/$(LIBRARY)
$(LIBDIR)/$(LIBRARY): $(LIBRY)
	cp $(LIBRY) $(LIBDIR)
	chmod 664 $(LIBDIR)/$(LIBRARY)
	catalog.csh proc $(SRCDIR) c > /dev/null

$(LIBRY): $(OBJ)
	$(AR) $(LIBRY) $?

%.o: %.c
	$(CC) -c -o $@ $<

clean :
	rm -f $(OBJ) *.BAK *~ #*# dependlist;

bare: clean
	rm -f $(LIBRY)

$(MAKEFILE): $(SRC)
	$(CC) -xM $(SRC) > dependlist
	@sed -e '1,/^# DO NOT DELETE/!d' $(MAKEFILE) > $(MAKEFILE).tmp.$$$$; \
	cat dependlist >> $(MAKEFILE).tmp.$$$$; \
	cp $(MAKEFILE) $(MAKEFILE).BAK; \
	mv $(MAKEFILE).tmp.$$$$ $(MAKEFILE); \
	rm -f dependlist

depend: $(SRC)
	$(CC) -xM $(SRC) > dependlist
	@sed -e '1,/^# DO NOT DELETE/!d' $(MAKEFILE) > $(MAKEFILE).tmp.$$$$; \
	cat dependlist >> $(MAKEFILE).tmp.$$$$; \
	cp $(MAKEFILE) $(MAKEFILE).BAK; \
	mv $(MAKEFILE).tmp.$$$$ $(MAKEFILE); \
	rm -f dependlist; \
	echo make depend complete

instarch : bare
	@case "$(ARCHTYPE)" in \
	osf) rm -f byte2bit.c; \
	     cp byte2bit.$(ARCHTYPE) byte2bit.c; \
             chmod 444 byte2bit.c; \
	     rm -f histgram.c; \
	     cp histgram.$(ARCHTYPE) histgram.c; \
             chmod 444 histgram.c; \
	     rm -f makefile.mak; \
	     cp makefile.$(ARCHTYPE) makefile.mak; \
	     chmod 640 makefile.mak; \
	     echo $(ARCHTYPE) files installed \
	;; \
	sun|sol|sgi|aix|hp) rm -f byte2bit.c; \
	     cp byte2bit.sun byte2bit.c; \
             chmod 444 byte2bit.c; \
	     rm -f histgram.c; \
	     cp histgram.sun histgram.c; \
             chmod 444 histgram.c; \
	     rm -f makefile.mak; \
	     cp makefile.$(ARCHTYPE) makefile.mak; \
	     chmod 640 makefile.mak; \
	     echo $(ARCHTYPE) files installed \
	;; \
	*) echo 'Tried to install uknown machine architecture: '$(ARCHTYPE); \
	   exit 1 \
	;; \
	esac

# DO NOT DELETE THIS LINE - make depend uses it
