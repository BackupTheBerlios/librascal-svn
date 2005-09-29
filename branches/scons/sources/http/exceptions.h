// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#ifndef __rascal_httpclient_exceptions_h
#define __rascal_httpclient_exceptions_h

#include <string>
#include "http.h"

using namespace rascal;

class genhttpex : public httpexception
{
	std::string mText;
public:
	genhttpex(const std::string &txt) { mText = txt; }
	~genhttpex() { }
	const char * message() { return mText.c_str(); }
};

#endif
