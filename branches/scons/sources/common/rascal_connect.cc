// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include <errno.h>
#include "common.h"
#include "debug.h"

RASCAL_API(rrid_t) rascal_connect(const sock_t *target, rascal_dispatcher disp, void *context, const char *proto)
{
	return rascal_connect_ex(target, disp, context, proto);
}
