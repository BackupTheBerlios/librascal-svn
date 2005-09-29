// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#ifndef __rascal_http_clidata_h
#define __rascal_http_clidata_h

#include <list>
#include <string>
#include <vector>
#include "plus.h"
#include "de.h"

typedef std::pair<std::string, std::string> header;
typedef std::list<header> headers;

namespace rascal
{

class httpclient;

class clidata : public conn
{
	// The real client, to which report events to.
	httpclient *mClient;
	// Transfer decoder.
	debase *mTransferDecoder;
	// Request method.
	std::string mMethod;
	// Headers to send to the server.
	headers mOHeaders;
	// Headers received from the server.
	headers mIHeaders;
	// Set when response code is known.
	bool mGotCode;
	// Set when all headers have arrived.
	bool mGotHeaders;
	// Global initialization flag.
	static bool mRascalReady;
private:
	// Send the request.
	bool send_request();
	// Send headers.
	bool send_headers();
	// Decode response code.
	bool decode_response();
	// Decode incoming headers.
	bool decode_headers();
public:
	std::string mProto;
	std::string mServer;
	std::string mPort;
	std::string mQuery;
public:
	clidata(httpclient *cli);
	~clidata();
	// Parse an URL.
	void parse(std::string url, const std::string &method);
	// Start processing the request.
	void go();
	// Append headers.
	void add_header(const char *name, const char *value);
	// Report errors.
	void report(const char *, rrid_t);
	void report(const char *msg) { mClient->error(msg); }
	// Pass data to the client.
	void pass(void *at, unsigned int sz) { mClient->on_data(at, sz); }
protected:
	bool on_connect(rrid_t rc, const sock_t &peer);
	void on_write();
	void on_read();
	void on_close(rrid_t rc);
};

}

#endif
