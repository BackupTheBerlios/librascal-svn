// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#include <stdio.h>
#include "client.h"

client::client()
{
	mBusy = true;
}

client::~client()
{
}

void client::on_response(unsigned int code, const char *text)
{
	printf(" - response: %u (%s)\n", code, text);
}

// Called when headers arrive.
void client::on_header(const char *name, const char *value)
{
	printf(" -   header: %s => %s\n", name, value);
}

// Called when data arrives.
void client::on_data(void *buffer, size_t size)
{
	if (size)
		printf(" - on_data(%u)\n", size);
	else {
		printf(" - end.\n");
		mBusy = false;
	}
}

// Override to add more headers to the request.
void client::add_headers()
{
	add_header("Accept-Encoding", "*/*");
	add_header("Accept-Charset", "*");
}

// Override to send data.  Set size=0 to stop.
void client::add_data(void *buffer, size_t &size)
{
	printf(" - add_data(%u)\n", size);
}

void client::error(const char *msg)
{
	printf(" ! error: %s.\n", msg);
	mBusy = false;
}
