// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#include <stdio.h>
#include <unistd.h>
#include "client.h"

int main(int argc, const char **argv)
{
	client cli;
	const char *url;

#ifdef _DEBUG
	if (argc < 2)
		url = "http://cjlohbi.su/fubar/2";
#else
	if (argc < 2) {
		fprintf(stderr, "Usage: rahttp url\n");
		return 1;
	} else {
		url = argv[1];
	}
#endif

	cli.request(url);

	while (cli.busy())
		sleep(1);

	return 0;
}
