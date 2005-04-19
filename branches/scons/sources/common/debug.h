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
# define debug(str) flog_write str
extern flog_t flog;
void debug_init(void);
#else
# define debug(str)
# define debug_init()
#endif

#endif // __common_debug_h
