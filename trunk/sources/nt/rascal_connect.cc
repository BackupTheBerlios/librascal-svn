// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include "connection.h"
#include "connection_dg.h"
#include "connection_st.h"

rrid_t rascal_connect_ex(const sock_t *target, rascal_dispatcher disp, void *context, int mode)
{
	rrid_t rc;
	connection *tmp;

	if (mode == SOCK_DGRAM)
		tmp = new connection_dg(disp, context);
	else
		tmp = new connection_st(disp, context);

	if (tmp == NULL)
		return GetLastError() | REC_SYSERROR_MASK;

	if (!rascal_isok(rc = tmp->connect(*target))) {
		delete tmp;
		return rc;
	}

	return tmp->get_id();
}


rrid_t rascal_connect(const sock_t *target, rascal_dispatcher disp, void *context)
{
	return rascal_connect_ex(target, disp, context, SOCK_STREAM);
}
