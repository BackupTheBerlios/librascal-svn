// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#include "cache.h"

bool cid::operator == (const cid &src) const
{
	if (!mProto.empty()) {
		if (mProto != src.mProto)
			return false;
	} else if (mPort != src.mPort)
		return false;
	if (mHost != src.mHost)
		return false;
	return true;
}

cid& cid::operator = (const cid &src)
{
	mProto = src.mProto;
	mHost = src.mHost;
	mPort = src.mPort;
	return *this;
}

httpcache::httpcache()
{
}

httpcache::~httpcache()
{
}

bool httpcache::find(const std::string &url, rascal::conn &c)
{
	const char *proto = NULL;
}
