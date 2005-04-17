# Makefile for GNU Make.
# $Id: GNUmakefile 14 2005-01-15 23:05:00Z hex $

-include Makedefs.faerion.include

DIRS = sources/$(MDF_LIBARCH)
PACK = librascal
VERS = 2.0
PREQ = configure

ifneq ($(WINDIR),)
  PACKAGES = $(PACK)-$(VERS).zip
endif

ifeq ($(FLAVOR),full)
  DIRS += sources/samples
endif

include Makedefs.faerion

fake: all
	rm -rf fake-$(VERS)
	mkdir -p fake-$(VERS)/bin
	mkdir -p fake-$(VERS)/include
	mkdir -p fake-$(VERS)/lib
	mkdir -p fake-$(VERS)/share/rascal
	cp LICENSE fake-$(VERS)/share/rascal
	cp sources/rascal.h fake-$(VERS)/include/
	cp sources/$(MDF_LIBARCH)/librascal.$(MDF_SOEXT) fake-$(VERS)/lib
ifeq ($(MDF_OSNAME),NT)
	cp sources/$(MDF_LIBARCH)/librascal.$(MDF_SOEXT).a fake-$(VERS)/lib
endif
	for i in `find sources/samples -type f -perm -0100`; do cp $$i fake-$(VERS)/bin; done

install: fake
  ifeq ($(PREFIX),)
	@echo "The PREFIX envar is not defined."
  else
	cp -R fake-$(VERS)/* $(PREFIX)/ && rm -rf fake-$(VERS)
  endif
