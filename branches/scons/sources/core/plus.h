// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
//
// The complete documentation can be found at the following location:
// <http://faerion.sourceforge.net/doc/rascal/>
//
// The library is distributed under the terms of GNU LGPL.
//
// $Id$

#ifndef __rascal_plus_h
#define __rascal_plus_h

#include "api.h"

namespace rascal
{

class errmsg
{
	char tmp[1024];
public:
	errmsg(rrid_t rid) { rascal_get_errmsg(rid, tmp, sizeof(tmp)); }
	const char * c_str() const { return tmp; }
};

class ntoa
{
	char tmp[256];
public:
	ntoa(const sock_t &peer) { rascal_ntoa(&peer.addr, tmp, sizeof(tmp)); }
	ntoa(const addr_t &addr) { rascal_ntoa(&addr, tmp, sizeof(tmp)); }
	const char * c_str() const { return tmp; }
};

};

#endif
