// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include "connection.h"

RASCAL_API(rrid_t) rascal_work(unsigned int msec)
{
	return connection::work(msec);
}
