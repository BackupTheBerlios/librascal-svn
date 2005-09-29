// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#include "clidata.h"
#include "de.h"

debase::debase(clidata *cli)
{
	mClient = cli;
}

debase::~debase()
{
}

void debase::pass()
{
	rrid_t rc;
	char tmp[1024];

	for (unsigned int sz = sizeof(tmp); rascal_isok(rc = mClient->read(tmp, sz)); sz = sizeof(tmp))
		mClient->pass(tmp, sz);
}
