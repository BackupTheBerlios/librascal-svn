// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$
//
// Installing the service is done by series of tricks.
//
// First, we pretend that we connect to a service, but the filter will
// decline all attempts, attempting to accept on that socket.  Events
// from the listeners are routed to a local event router, which replaces
// the request id with the original one and calls the user handler.

#include <faeutil/list.h>
#include "object.h"
#include "debug.h"

// Request information.
struct ris_data
{
	// Request handle.
	rrid_t rid;
	// Original context.
	void *context;
	// Original handler.
	rascal_dispatcher disp;
};

// The filter acts as an event router this time.
static bool __rascall ris_filter(void *context, const sock_t *addr)
{
	rrid_t rid;
	ris_data *ris = reinterpret_cast<ris_data *>(context);

	if (rascal_isok(rid = rascal_accept(addr, ris->disp, ris->context))) {
		if (!ris->disp(rid, addr, rop_listen, ris->context))
			rascal_cancel(rid);
	}

	return false;
}

static bool __rascall ris_disp(rrid_t, const sock_t *, int, void *context)
{
	delete reinterpret_cast<ris_data *>(context);
	return false;
}

rrid_t rascal_accept_service(const char *name, int proto, const char *domain, rascal_dispatcher disp, void *context)
{
	rrid_t rid;
	ris_data *ris = new ris_data;

	ris->context = context;
	ris->disp = disp;

	if (!rascal_isok(rid = ris->rid = rascal_connect_service(name, proto, domain, ris_disp, ris, ris_filter)))
		delete ris;

	return rid;
}
