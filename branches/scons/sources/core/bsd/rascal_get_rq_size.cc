// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include "connection.h"
#include "../common/debug.h"

rrid_t rascal_get_rq_size(rrid_t rid, unsigned int *size)
{
	pobject<connection> tmp(rid);

	if (!tmp.is_valid())
		return REC_INVALID_HANDLE;

	*size = tmp->get_rq_size();

	debug((flog, rl_misc, "reporting %u bytes in read queue for connection %X", *size, tmp->get_id()));
	return REC_SUCCESS;
}
