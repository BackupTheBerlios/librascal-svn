// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#ifndef __rascal_rahttp_client_h
#define __rascal_rahttp_client_h

#include "../http/http.h"

using namespace rascal;

class client : public httpclient
{
	bool mBusy;
public:
	client();
	~client();
	bool busy() const { return mBusy; }
protected:
	// Called when the response arrives.
	void on_response(unsigned int code, const char *text);
	// Called when headers arrive.
	void on_header(const char *name, const char *value);
	// Called when data arrives.
	void on_data(void *buffer, size_t size);
	// Override to add more headers to the request.
	void add_headers();
	// Override to send data.  Set size=0 to stop.
	void add_data(void *buffer, size_t &size);
	// Catch errors.
	void error(const char *);
};

#endif
