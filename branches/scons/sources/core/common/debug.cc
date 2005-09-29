// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#if defined(_DEBUG) && defined(HAVE_flog)

#include <flog.h>
#include <stdarg.h>
#include <stdio.h>
#include "debug.h"

static flogdef_t logs[] =
{
	{ "misc", "librascal.log", 1, 1, 1 },
	{ "monitor", "librascal.log", 1, 1, 1 },
	{ "res", "librascal.log", 1, 1, 1 },
	{ "conn", "librascal.log", 1, 1, 1 },
	{ "conn-svc", "librascal.log", 1, 1, 1 },
	{ "accept", "librascal.log", 1, 1, 1 },
	{ "accept-svc", "librascal.log", 1, 1, 1 },
	{ "interface", "librascal.log", 1, 1, 1 },
	{ "iocp", "librascal.log", 1, 1, 1 }
};

flog_t flog = 0;

void debug_init(void)
{
	if ((flog = flog_init(logs, sizeof(logs) / sizeof(logs[0]))) != 0)
		flog_write(flog, 0, "debugging initialized");
	else {
		fprintf(stderr, "could not open librascal.log\n");
	}
}

void radebug(flog_t f, int level, const char *format, ...)
{
	va_list vl;
	char msg[4096];

	va_start(vl, format);
	vsnprintf(msg, sizeof(msg), format, vl);
	va_end(vl);

	msg[sizeof(msg)-1] = 0;
	flog_write(f, level, msg);
}

#endif
