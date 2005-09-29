// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2005 hex@faerion.oss
// Distributed under the BSD License.
//
// $Id$
//
// Declarations for transfer-encoding processors.

#ifndef __rascal_http_de_h
#define __rascal_http_de_h

#include <vector>
#include "http.h"

using namespace rascal;

class rascal::clidata;

class debase
{
protected:
	clidata *mClient;
public:
	debase(clidata *c);
	virtual ~debase();
	virtual void pass();
};

class dechunk : public debase
{
	// The number of chunks yet to receive.  This is decremented
	// on each new chunk declaration, but reset to a random big
	// value when a non-empty chunk is expected.
	unsigned int mChunksToGo;
	// Chunk data, may be partial.
	std::vector<char> mChunk;
	// The size of the next chunk.
	unsigned int mChunkSize;
	// Decode chunk size.
	bool decode_size(const char *str);
	// Add currently available data to the chunk.
	bool add(unsigned int sz);
public:
	dechunk(clidata *);
	~dechunk();
	void pass();
};

#endif
