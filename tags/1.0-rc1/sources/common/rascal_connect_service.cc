// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include <faeutil/faeutil.h>
#include "object.h"
#include "debug.h"
#include "resolver/resolver.h"

rrid_t rascal_connect_service(const char *name, int proto, const char *domain, rascal_dispatcher disp, void *context, rascal_rcs_filter filter)
{
	rrid_t rid;
	char hname[256];
	const char *pname;

	if (proto == proto_tcp)
		pname = "tcp";
	else if (proto == proto_udp)
		pname = "udp";
	else
		return REC_INVALID_ARGUMENT;

	strlcpya(hname, sizeof(hname), "_", name, "._", proto, ".", domain, NULL);

	new getsrv(hname, disp, context, filter, rid);
	return rid;
}
