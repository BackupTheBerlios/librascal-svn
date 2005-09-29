// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#ifndef __rascal_httpclient_cache_h
#define __rascal_httpclient_cache_h

#include <list>
#include <string>
#include "plus.h"

struct cid
{
	std::string mProto;
	std::string mHost;
	unsigned short mPort;
	cid(const cid &src) { *this = src; }
	cid& operator = (const cid &src);
	bool operator == (const cid &src) const;
};

class httpcache
{
	std::list<cid> mCache;
public:
	httpcache();
	~httpcache();
	// Find a connection.
	bool find(const std::string &url, rascal::conn &c);
};

#endif
