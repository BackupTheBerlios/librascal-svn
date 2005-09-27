// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include <stdio.h>
#include "object.h"
#include "debug.h"

RASCAL_API(rrid_t) rascal_wait(rrid_t rid)
{
	pobject<> tmp(rid);

	if (!tmp.is_valid()) {
		debug((flog, rl_interface, "%X: refusing to wait for an invalid object", rid));
		return REC_INVALID_HANDLE;
	}

	debug((flog, rl_interface, "%X: waiting for object", rid));
	tmp->wait();
	debug((flog, rl_interface, "%X: waiting finished", rid));

	return REC_SUCCESS;
}
