# Makefile for GNU Make.
# $Id: GNUmakefile 21 2005-01-23 00:03:02Z hex $

include ../../../Makedefs.faerion.include

TARGET  = executable
PROG    = raconn
DEFS   += RASCAL_HELPERS
SRCS    = $(wildcard *.cc)
LIBS   += rascal faeutil stdc++
LIBDIR += ../../$(MDF_LIBARCH)

ifeq ($(MDF_OSNAME),NT)
  PROG := $(PROG).exe
  SRCS += $(wildcard *.rc)
  LIBS += pthreadGCE
else
  LIBS += pthread
endif

include ../../../Makedefs.faerion

debug: all
	@echo " "
	echo "tb main" > /tmp/raconn.gdb
	echo "r -S irc tcp cjlohbi.su" >> /tmp/raconn.gdb
	gdb -x /tmp/raconn.gdb $(PROG)
