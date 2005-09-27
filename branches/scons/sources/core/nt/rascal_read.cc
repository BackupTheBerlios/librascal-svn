// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include "connection.h"

RASCAL_API(rrid_t) rascal_read(rrid_t rid, char *to, unsigned int *size)
{
	pobject<connection> tmp(rid);

	if (!tmp.is_valid())
		return REC_INVALID_HANDLE;

	return tmp->read(to, *size);
}


RASCAL_API(rrid_t) rascal_reads(rrid_t rid, char *to, unsigned int *size, int flags)
{
	pobject<connection> tmp(rid);

	if (!tmp.is_valid())
		return REC_INVALID_HANDLE;

	return tmp->reads(to, *size, flags);
}
