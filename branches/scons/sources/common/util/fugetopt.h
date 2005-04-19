// Faerion RunTime Library.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#ifndef __librascal_common_util_fugetopt_h
#define __librascal_common_util_fugetopt_h

#if defined(HAVE_getopt)
# include <getopt.h>
#else

extern int opterr, optind, optopt, optreset;
extern char *optarg;
extern const char *__program;
int getopt(int nargc, char * const *nargv, const char *options);

#endif

#endif
