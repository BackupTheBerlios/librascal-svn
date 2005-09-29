// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$
//
// This header defines the base (abstract) HTTP client and server
// classes.  Subclass to implement real clients and servers.

#ifndef __rascal_http_h
#define __rascal_http_h

#include "plus.h"

namespace rascal
{

class clidata;

class httpclient
{
	clidata *mData;
	friend class clidata;
public:
	httpclient();
	virtual ~httpclient();
	// Put a request.
	void request(const char *url, const char *method = "GET");
protected:
	// Called when response code arrives.
	virtual void on_response(unsigned int code, const char *text) = 0;
	// Called when headers arrive.
	virtual void on_header(const char *name, const char  *value) = 0;
	// Called when data arrives.
	virtual void on_data(void *buffer, size_t size) = 0;
	// Add more headers.
	void add_header(const char *name, const char *value);
protected:
	// Override to add more headers to the request.
	virtual void on_connect() { }
	// Override to send data.  Set size=0 to stop.
	virtual void add_data(void *buffer, size_t &size);
	// Receives error messages.
	virtual void error(const char *) = 0;
};

};

#endif
