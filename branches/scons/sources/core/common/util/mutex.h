// Faerion RunTime Library.
// Copyright (c) 2003-2005 hex@faerion.oss and others.
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#ifndef __librascal_common_util_mutex_h
#define __librascal_common_util_mutex_h

#include <pthread.h>

namespace funs {

class mutex
{
public:
	// Data members.
	pthread_mutex_t lock;
	// Code members.
	mutex();
	~mutex();
	void enter();
	void leave();
	operator pthread_mutex_t * () { return &lock; }
};


class mlock
{
	mutex &host;
public:
	mlock(mutex &object) : host(object) { host.enter(); }
	~mlock() { host.leave(); }
};


class tlskey
{
	pthread_key_t _host;
public:
	tlskey();
	~tlskey();
	bool set(const void *arg);
	void * get();
};

}

using namespace funs;

#endif // __librascal_common_util_mutex_h
