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

run: all
	@echo " "
	rm -f raconn.rd librascal.log
	./$(PROG) -a 192.168.0.1 -p 143 -r raconn.rd -w raconn.wr

debug: all
	@echo " "
	rm -f raconn.rd librascal.log
	gdb -x raconn.gdb $(PROG)
