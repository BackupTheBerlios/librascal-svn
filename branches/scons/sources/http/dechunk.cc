// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$

#include "clidata.h"
#include "de.h"

dechunk::dechunk(clidata *cli) :
	debase(cli)
{
	mChunksToGo = 666;
	mChunkSize = 0;
}

dechunk::~dechunk()
{
}

void dechunk::pass()
{
	rrid_t rc;
	unsigned int sz;

	while (mChunksToGo) {
		// Receive chunk size.
		if (mChunkSize == 0) {
			char tmp[1024];
			unsigned int sz = sizeof(tmp);

			if (!rascal_isok(rc = mClient->reads(tmp, sz))) {
				mClient->report("read", rc);
				return;
			}

			decode_size(tmp);
		}

		// Receive chunked data.
		if (!rascal_isok(rc = mClient->get_rq_size(sz))) {
			mClient->report("read", rc);
			return;
		}

		// Make sure we don't peek too much.
		if (sz > mChunkSize)
			sz = mChunkSize;

		if (!add(sz)) {
			mClient->report("add_to_chunk() failed");
			return;
		}
	}
}

bool dechunk::decode_size(const char *str)
{
	if (sscanf(str, "%x", &mChunkSize) != 1) {
		mClient->report("invalid chunk size");
		return false;
	}

	// If this is not the last chunk, bounce.
	if (mChunkSize != 0)
		mChunksToGo = 666;
	else
		mChunksToGo = 1;

	// Include \r\n.
	mChunkSize += 2;

	return true;
}

bool dechunk::add(unsigned int sz)
{
	rrid_t rc;
	size_t csz = mChunk.size();

	mChunk.resize(csz + sz);

	if (!rascal_isok(rc = mClient->read(&mChunk[csz], sz))) {
		mClient->report("read", rc);
		return false;
	}

	if (mChunk.size() != csz + sz) {
		mClient->report("(rascal) read error");
		return false;
	}

	if ((mChunkSize -= sz) == 0) {
		mClient->pass(&mChunk[0], mChunk.size() - 2);
		mChunksToGo--;
		mChunk.clear();
	}
	
	return true;
}
