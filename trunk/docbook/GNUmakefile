# Makefile for GNU Make.

include ../Makedefs.faerion.include

TARGET  = docbook.paged
DOCBOOK = xhtml
DOCNAME = rascal
DOCSUPP = rascal.css
DEFS    = chapter.autolabel=1 html.stylesheet=rascal.css chunk.section.depth=3 chunk.first.sections=1 use.id.as.filename=1 html.extra.head.links=1 toc.section.depth=3 section.autolabel=1 section.label.includes.component.label=1 css.decoration=0
SRCS    = rascal.docbook
DEPS    = $(wildcard app.*.docbook ch.*.docbook)

ifneq ($(MDF_XSLTPROC),)
  include ../Makedefs.faerion
endif
