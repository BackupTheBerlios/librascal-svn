// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#ifndef __common_debug_h
#define __common_debug_h

enum raloglev
{
	rl_misc,
	rl_monitor,
	rl_resolver,
	rl_conn,
	rl_conn_svc,
	rl_accept,
	rl_accept_svc,
	rl_interface,
	rl_iocp,
};

#if defined(_DEBUG) && defined(HAVE_flog)
# include <flog.h>
# define xdebug(str) flog_write str
# define debug(str) radebug str
extern flog_t flog;
void debug_init(void);
void radebug(flog_t f, int level, const char *, ...);
#else
# define debug(str)
# define debug_init()
#endif

#endif // __common_debug_h
