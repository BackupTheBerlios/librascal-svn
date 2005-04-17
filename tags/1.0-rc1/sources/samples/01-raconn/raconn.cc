// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$
//
// Test case for outgoing connections, reading and writing.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
# include <windows.h>
# include <io.h>
#else
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <faeutil/faeutil.h>
#include <faeutil/sem.h>
#include <rascal.h>

#ifndef dimof
# define dimof(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

using namespace rascal;
using namespace faeutil;

static const char *usage_msg =
	"This is a test program for RASCAL, which establishes an outgoing\n"
	"connection and optionally reads and writes to it.  The program\n"
	"displays significant amount of debugging information.\n"
	"\n"
	"Usage: raconn [OPTIONS]\n"
	"\n"
	"Options:\n"
	"  -a addr  : connect to this numeric address\n"
	"  -h host  : connect to this host name\n"
	"  -p port  : connect to this port number\n"
	"  -r file  : write incoming data to this file (default: raconn.in)\n"
	"  -w file  : send the content of this file (default: raconn.out)\n"
	"\n"
	"Send bug reports to <team@faerion.oss>\n"
	;

static const char *events[] = { "rop_accept", "rop_close", "rop_connect", "rop_read", "rop_write" };

static const char *file_rd = "raconn.in";
static const char *file_wr = "raconn.out";

static sem hsem;
static rrid_t rid = -1;

class addrlist
{
public:
	addr_t list[16];
	unsigned int count;
	addrlist()
	{
		count = 0;
	}
};

bool send_wr(rrid_t conn)
{
	FILE *in;
	bool rc = true;
	struct stat fs;

	if ((in = fopen(file_wr, "rb")) == NULL)
		return true;

	if (fstat(fileno(in), &fs) != 0) {
		fclose(in);
		return false;
	}

	if (fs.st_size != 0) {
		rrid_t err;
		void *tmp = malloc(fs.st_size);

		fread(tmp, fs.st_size, 1, in);

		if (!rascal_isok(err = rascal_write(conn, (char*)tmp, fs.st_size))) {
			printf("Could not write to the connection, %s\n", errmsg(err).c_str());
			rc = false;
		} else {
			printf("Queueed %u bytes for writing.\n", static_cast<unsigned int>(fs.st_size));
		}

		free(tmp);
	}

	fclose(in);
	return rc;
}


static bool read_rd(rrid_t conn)
{
	FILE *in;
	rrid_t rc;
	char *data;
	unsigned int size;

	if ((in = fopen(file_rd, "ab")) == NULL) {
		printf("Could not open file %s for writing.\n", file_wr);
		return true;
	}

	while (rascal_isok(rascal_get_rq_size(conn, &size)) && size != 0) {
		data = (char*)malloc(size);
		if (data == NULL) {
			printf("Memory allocation error, could not save read data.\n");
			fclose(in);
			return false;
		}

		if (!rascal_isok(rc = rascal_reads(conn, data, &size, RRF_NORMAL /* RRF_UNTRIMMED */))) {
			printf("Could not retreive read data: 0x%08x: %s.\n", rc, errmsg(rc).c_str());
			free((void*)data);
			fclose(in);
			return false;
		}

		printf("Writing %u bytes to %s: %s\n", size, file_rd, data);
		fwrite(data, size, 1, in);
		free((void*)data);
	}

	fclose(in);
	return true;
}


static bool __rascall dispatcher(rrid_t conn, const sock_t *peer, int event, void *context)
{
	printf("dispatcher,\n  > conn=%x,\n  > peer=%s:%u,\n  > event=%d(%s),\n  > context=0x%08x.\n", conn, ntoa(*peer).c_str(), peer->port, event, events[event], (int)context);

	switch (event) {
	case rop_connect:
		if (rascal_isok(conn)) {
			printf("Connected to %s:%u.\n", ntoa(*peer).c_str(), peer->port);
			return send_wr(conn);
		} else {
			printf("Could not connect to %s:%u, %s\n", ntoa(*peer).c_str(), peer->port, errmsg(conn).c_str());
			hsem.post();
		}
		break;
	case rop_close:
		if (conn == REC_SUCCESS)
			printf("Connection to %s:%u gracefully closed.\n", ntoa(*peer).c_str(), peer->port);
		else
			printf("Connection to %s:%u closed: %s\n", ntoa(*peer).c_str(), peer->port, errmsg(conn).c_str());
		hsem.post();
		break;
	case rop_read:
		printf("Read data.\n");
		return read_rd(conn);
	case rop_write:
		{
			unsigned int left;
			if (rascal_isok(rascal_get_sq_size(conn, &left)))
				printf("Wrote data, bytes left: %u.\n", left);
			else
				printf("Wrote data, queue size is unknown.\n");
		}
		break;
	}

	return false;
}


void do_connect(const addr_t &addr, unsigned short port)
{
	char host[64];
	rrid_t conn;
	sock_t peer;
	rascal_ntoa(&addr, host, sizeof(host));

	peer.addr = addr;
	peer.port = port;

	if (rascal_isok(conn = rascal_connect(&peer, dispatcher))) {
		printf("Connection request to %s:%u accepted, rrid=0x%08x.\n", host, peer.port, conn);
	} else {
		fprintf(stderr, "Connection to %s:%u failed: %08X: %s\n", host, peer.port, conn, errmsg(rid).c_str());
		return;
	}

	hsem.wait();
	printf("Outgoing connection test finished.\n");
}


static void __rascall on_getaddr(void *context, const char *host, unsigned int count, const addr_t *addrs)
{
	addrlist &list = *(addrlist *)context;

	printf("resolved %s to %u addresses.\n", host, count);

	list.count = 0;

	if (count >= dimof(list.list))
		count = dimof(list.list);

	for (unsigned int idx = 0; idx < count; ++idx) {
		char tmp[64];
		rascal_ntoa(addrs + idx, tmp, sizeof(tmp));
		printf("  %02u. %s\n", idx+1, tmp);

		list.list[list.count++] = addrs[idx];
	}

	hsem.post();
}


int main(int argc, char *argv[])
{
	addrlist list;
	unsigned short port = 0;
	const char *host = NULL;

	for (int c; (c = getopt(argc, argv, "a:h:p:r:w:")) > 0; ) {
		switch (c) {
		default:
			fprintf(stderr, "%s", usage_msg);
			return 1;
		case 'a':
			if (list.count == dimof(list.list)) {
				fprintf(stderr, "raconn: address limit reached.\n");
				return 1;
			}

			if (!rascal_aton(optarg, &list.list[list.count++])) {
				fprintf(stderr, "raconn: invalid numeric address: %s.\n", optarg);
				return 1;
			}
			break;
		case 'h':
			host = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'r':
			file_rd = optarg;
			break;
		case 'w':
			file_wr = optarg;
			break;
		}
	}

	if (port == 0) {
		fprintf(stderr, "Port number not specified (try raconn -?).\n");
		return 1;
	}

	if (!rascal_isok(rid = rascal_init(RIP_WORKER_SINGLE))) {
		fprintf(stderr, "RASCAL initialization failed: %s\n", errmsg(rid).c_str());
		return 1;
	}

	if (host != NULL) {
		printf("Resolving %s.\n", host);

		if (!rascal_isok(rid = rascal_getaddr(host, on_getaddr, (void*)&list))) {
			fprintf(stderr, "Could not resolve %s: 0x%08X: %s\n", host, rid, errmsg(rid).c_str());
			return 1;
		}

		hsem.wait();

		if (list.count == 0) {
			fprintf(stderr, "Could not resolve %s.\n", host);
			return 1;
		}

		printf("Hostname resolution finished.\n");
	} else if (list.count == 0) {
		fprintf(stderr, "Nowhere to connect.\n");
		return 1;
	}

	for (unsigned int idx = 0; idx < list.count; ++idx)
		do_connect(list.list[idx], port);

	fprintf(stdout, "Waiting one second to flush the logs.\n");
	sleep(1);

	return 0;
}
