// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#include <ctype.h>
#include <string.h>
#include "clidata.h"
#include "http.h"

using namespace rascal;

bool clidata::mRascalReady = false;

clidata::clidata(httpclient *cli)
{
	mClient = cli;
	mTransferDecoder = NULL;
}

clidata::~clidata()
{
}

void clidata::report(const char *msg, rrid_t rc)
{
	std::string tmp = std::string(msg) + " error: " + errmsg(rc).c_str();
	mClient->error(tmp.c_str());
}

void clidata::parse(std::string url, const std::string &method)
{
	size_t sz;

	// Check the protocol and remove it from the url.
	if (url.substr(0, 7) == "http://") {
		mProto = "http";
		url.erase(0, 7);
	} else {
		mClient->error("unsupported protocol");
		return;
	}

	// Extract the mServer part.
	if ((sz = url.find('/')) >= url.length()) {
		mServer = url;
		url = "/";
	} else {
		mServer = url.substr(0, sz);
		url.erase(0, sz);
	}

	// Extract the mPort number.
	if ((sz = mServer.find(':')) < mServer.length()) {
		mPort = mServer.substr(sz + 1, mServer.length() - sz - 1);
		mServer.erase(sz);
	}

	if (mServer.empty()) {
		mClient->error("server name not specified");
		return;
	}

	mQuery = url;

	mMethod = method;
	add_header("Host", mServer.c_str());
	add_header("X-RASCAL-HTTP-Client", "$Id$");
}

void clidata::go()
{
	rrid_t rc;

	mGotCode = false;
	mGotHeaders = false;

	if (!mRascalReady) {
		if (!rascal_isok(rc = rascal_init(RIP_WORKER_PER_CPU))) {
			report("initialization", rc);
			return;
		}
		mRascalReady = true;
	}

	rc = conn::connect_service(mProto.c_str(), "tcp", mServer.c_str());

	if (!rascal_isok(rc)) {
		report("connection", rc);
		return;
	}
}

bool clidata::on_connect(rrid_t rc, const sock_t &peer)
{
	if (!rascal_isok(rc)) {
		report("connect", rc);
		return false;
	}

	mClient->on_connect();

	if (!send_request())
		return false;
	if (!send_headers())
		return false;

	if (!rascal_isok(rc = write("\n", 1))) {
		report("write", rc);
		return false;
	}

	return true;
}

void clidata::on_write()
{
}

void clidata::on_read()
{
	rrid_t rc;
	unsigned int sz;
	char tmp[4096], *p;

	if (!mGotCode) {
		sz = sizeof(tmp);
		rc = reads(tmp, sz);

		if (strncmp(tmp, "HTTP/1.", 7) != 0) {
			mClient->error("unsupported HTTP version");
			return;
		}

		for (p = tmp; *p != ' ' && *p != 0; ++p);
		while (*p == ' ')
			++p;

		for (sz = 0; isdigit(*p); ++p)
			sz = sz * 10 + (*p - '0');

		while (*p == ' ')
			++p;

		mClient->on_response(sz, p);
		mGotCode = true;
	}

	while (!mGotHeaders) {
		for (unsigned int size = sizeof(tmp); rascal_isok(rc = reads(tmp, size, 0)); size = sizeof(tmp)) {
			char *colon;

			if (tmp[0] == 0) {
				mGotHeaders = true;
				break;
			}

			if ((colon = strchr(tmp, ':')) == NULL) {
				mClient->error("malformed header: no colon");
				mClient->error(tmp);
				return;
			}

			for (*colon++ = 0; *colon == ' '; *colon++ = 0);
			report_header(tmp, colon);
		}
	}

	if (!rascal_isok(rc = get_rq_size(sz))) {
		report("read", rc);
		return;
	}

	// Set the default decoder.  Not the best place for this,
	// should be moved somewhere where it won't be called on
	// every read.
	if (mTransferDecoder == NULL)
		mTransferDecoder = new debase(this);

	mTransferDecoder->pass();
}

void clidata::on_close(rrid_t rc)
{
	if (!rascal_isok(rc))
		report("generic", rc);
	else
		mClient->on_data(NULL, 0);
}

void clidata::add_header(const char *name, const char *value)
{
	mOHeaders.push_back(header(name, value));
}

// Send the request to the server.
bool clidata::send_request()
{
	rrid_t rc;
	std::string tmp;
	tmp = mMethod + " " + mQuery + " HTTP/1.1\n";

	if (!rascal_isok(rc = write(tmp.c_str(), tmp.length()))) {
		report("write", rc);
		return false;
	}

	return true;
}

// Send all headers to the server.
bool clidata::send_headers()
{
	rrid_t rc;
	std::string tmp;

	for (headers::const_iterator it = mOHeaders.begin(); it != mOHeaders.end(); ++it) {
		tmp = it->first + ": " + it->second + "\n";

		if (!rascal_isok(rc = write(tmp.c_str(), tmp.length()))) {
			report("write", rc);
			return false;
		}
	}

	return true;
}

void clidata::report_header(const char *name, const char *value)
{
	if (strcasecmp(name, "transfer-encoding") == 0 && strcasecmp(value, "chunked") == 0) {
		if (mTransferDecoder != NULL)
			return;
		mTransferDecoder = new dechunk(this);
	}
	mClient->on_header(name, value);
}
