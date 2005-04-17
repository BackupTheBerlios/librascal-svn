// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#ifndef __common_rascal_h
#define __common_rascal_h

#if defined(_WIN32)
# include <windows.h>
# include <winsock2.h>
#else
# include <sys/types.h>
# include <sys/socket.h>
#endif
#include <faeutil/mutex.h>
#include <faeutil/memory.h>
#include "../rascal.h"

#ifndef dimof
# define dimof(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#ifdef __GNUC__
# define ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
# define ATTRIBUTE_UNUSED
#endif

#ifdef _WIN32
# define GetSysError (REC_SYSERROR_MASK | GetLastError())
#else
# define GetSysError (REC_SYSERROR_MASK | errno)
#endif

// The function for establishing outgoing connections.
rrid_t rascal_connect_ex(const sock_t *target, rascal_dispatcher, void *context, int mode);

// Resolver initialization.
bool rascal_initres(void);

// Debug helper.
const char * get_event_name(int id);
void rascal_dumpfile(bool truncate, const void *src, unsigned int size, const char *format, ...);

#endif // __common_rascal_h
