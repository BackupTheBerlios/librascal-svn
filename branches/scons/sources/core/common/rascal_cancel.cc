// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include "object.h"
#include "debug.h"

RASCAL_API(rrid_t) rascal_cancel(rrid_t rid)
{
	pobject<> tmp(rid);

	if (!tmp.is_valid()) {
		debug((flog, rl_interface, "%X: attempted to cancel an invalid object", rid));
		return REC_INVALID_HANDLE;
	}

	tmp->cancel();

	debug((flog, rl_interface, "%X: object cancelled", rid));
	return REC_SUCCESS;
}
