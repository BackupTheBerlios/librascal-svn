// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include "connection.h"
#include "../common/debug.h"

RASCAL_API(rrid_t) rascal_write(rrid_t rid, const char *at, unsigned int size)
{
	pobject<connection> tmp(rid);

	if (!tmp.is_valid())
		return REC_INVALID_HANDLE;

	return tmp->write(at, size);
}
