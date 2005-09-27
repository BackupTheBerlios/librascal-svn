// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
//
// The complete documentation can be found at the following location:
// <http://faerion.sourceforge.net/doc/rascal/>
//
// The library is distributed under the terms of GNU LGPL.
//
// $Id$

#ifndef __rascal_plus_h
#define __rascal_plus_h

#include "api.h"

namespace rascal
{

class errmsg
{
	char tmp[1024];
public:
	errmsg(rrid_t rid) { rascal_get_errmsg(rid, tmp, sizeof(tmp)); }
	const char * c_str() const { return tmp; }
};

class ntoa
{
	char tmp[256];
public:
	ntoa(const sock_t &peer) { rascal_ntoa(&peer.addr, tmp, sizeof(tmp)); }
	ntoa(const addr_t &addr) { rascal_ntoa(&addr, tmp, sizeof(tmp)); }
	const char * c_str() const { return tmp; }
};

class conn
{
protected:
	rrid_t mConn;
public:
	conn()
	{
		mConn = REC_UNKNOWN_ERROR;
	}
	virtual ~conn()
	{
		if (rascal_isok(mConn))
			rascal_cancel(mConn);
	}
	void detach()
	{
		mConn = REC_UNKNOWN_ERROR;
	}
	rrid_t accept(const sock_t *addr)
	{
		return mConn = rascal_accept(addr, sdisp, this);
	}
	rrid_t cancel()
	{
		rrid_t rc = rascal_cancel(mConn);
		mConn = REC_UNKNOWN_ERROR;
		return rc;
	}
	rrid_t connect(const sock_t *target, const char *proto = 0)
	{
		return mConn = rascal_connect(target, sdisp, this, proto);
	}
	rrid_t connect_service(const char *name, const char *proto, const char *domain)
	{
		return mConn = rascal_connect_service(name, proto, domain, sdisp, this, sfilter);
	}
	rrid_t get_rq_size(unsigned int &size) const
	{
		return rascal_get_rq_size(mConn, &size);
	}
	rrid_t get_sq_size(unsigned int &size) const
	{
		return rascal_get_sq_size(mConn, &size);
	}
	rrid_t getaddr(const char *host)
	{
		return mConn = rascal_getaddr(host, sgetaddr, this);
	}
	rrid_t gethost(const addr_t *addr)
	{
		return mConn = rascal_gethost(addr, sgethost, this);
	}
	rrid_t accept_service(const char *name, const char *proto, const char *domain)
	{
		return mConn = rascal_accept_service(name, proto, domain, sdisp, this);
	}
	rrid_t read(char *to, unsigned int &size) const
	{
		return rascal_read(mConn, to, &size);
	}
	rrid_t reads(char *to, unsigned int &size, int flags = 0)
	{
		return rascal_reads(mConn, to, &size, flags);
	}
	rrid_t wait() const
	{
		return rascal_wait(mConn);
	}
	rrid_t write(const char *at, unsigned int size)
	{
		return rascal_write(mConn, at, size);
	}
protected:
	// Callbacks.
	virtual int on_disp(rrid_t conn, const sock_t *peer, int event)
	{
		return 1;
	}
	virtual int on_filter(const sock_t *addr)
	{
		return 1;
	}
	virtual void on_getaddr(const char *host, unsigned int count, const addr_t *addrs)
	{
	}
	virtual void on_gethost(const addr_t *addr, unsigned int count, const char **hosts)
	{
	}
private:
	// Callback wrappers.
	static int __rascall sdisp(rrid_t c, const sock_t *peer, int event, void *context)
	{
		return reinterpret_cast<conn *>(context)->on_disp(c, peer, event);
	}
	static int __rascall sfilter(void *context, const sock_t *addr)
	{
		return reinterpret_cast<conn *>(context)->on_filter(addr);
	}
	static void __rascall sgetaddr(void *context, const char *host, unsigned int count, const addr_t *addrs)
	{
		reinterpret_cast<conn *>(context)->on_getaddr(host, count, addrs);
	}
	static void __rascall sgethost(void *context, const addr_t *addr, unsigned int count, const char **hosts)
	{
		reinterpret_cast<conn *>(context)->on_gethost(addr, count, hosts);
	}
};

};

#endif
