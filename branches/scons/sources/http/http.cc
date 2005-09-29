// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#ifdef _DEBUG
# include <stdio.h>
#endif
#include <string>
#include "clidata.h"
#include "http.h"

namespace rascal
{

httpclient::httpclient()
{
	mData = NULL;
}

httpclient::~httpclient()
{
	if (mData != NULL)
		delete mData;
}

void httpclient::request(const char *url, const char *method)
{
	clidata *cli;

	if (mData != NULL) {
		delete mData;
		mData = NULL;
	}

	cli = new clidata(this);

	cli->parse(url, method);
	mData = cli;
	mData->go();
}

void httpclient::add_data(void *, size_t &)
{
}

void httpclient::add_header(const char *name, const char *value)
{
	if (mData != NULL)
		mData->add_header(name, value);
}

}
