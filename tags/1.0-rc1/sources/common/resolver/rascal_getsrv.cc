// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#include <string.h>
#include <faeutil/faeutil.h>
#include "resolver.h"
#include "../debug.h"

IMPLEMENT_ALLOCATORS(getsrv::srv);
IMPLEMENT_ALLOCATORS(getsrv::ina);

unsigned int getsrv::srv::depth() const
{
	unsigned int count = 1;

	for (srv *item = next; item != NULL; item = item->next)
		++count;

	return count;
}


unsigned int getsrv::ina::depth() const
{
	unsigned int count = 1;

	for (ina *item = next; item != NULL; item = item->next)
		++count;

	return count;
}


getsrv::getsrv(const char *hostname, rascal_dispatcher disp, void *context, rascal_rcs_filter filter, rrid_t &rc)
{
	get_context() = context;
	get_disp() = disp;
	get_filter() = filter;

	get_srv() = NULL;
	get_ina() = NULL;

	strlcpy(data, hostname, sizeof(data));

	if (!rascal_isok(rc = flush()))
		request::cancel();
	else
		rc = get_id();
}


getsrv::~getsrv()
{
	while (get_srv() != NULL) {
		srv *next = get_srv()->next;
		delete get_srv();
		get_srv() = next;
	}

	while (get_ina() != NULL) {
		ina *next = get_ina()->next;
		delete get_ina();
		get_ina() = next;
	}
}


bool getsrv::dump(char *&dst, unsigned int &size)
{
	header hdr(id);

	// Query count.
	hdr.qdcount = 1;

	if (!hdr.dump(dst, size))
		return false;

	// Insert the domain name.
	if (!hdr.put(dst, size, data))
		return false;

	// Insert the type of the query.
	if (!hdr.put(dst, size, (unsigned short)header::TYPE_SRV))
		return false;
	if (!hdr.put(dst, size, (unsigned short)header::CLASS_IN))
		return false;

	debug((flog, rl_resolver, "%X: built a getsrv dns packet %u (%s): qr=%u, opcode=%u, aa=%u, tc=%u, rd=%u, ra=%u, z=%u, rcode=%u, qdcount=%u, ancount=%u, nscount=%u, arcount=%u", get_id(), hdr.id, data, hdr.qr, hdr.opcode, hdr.aa, hdr.tc, hdr.rd, hdr.ra, hdr.z, hdr.rcode, hdr.qdcount, hdr.ancount, hdr.nscount, hdr.arcount));
	return true;
}


bool getsrv::on_record(header &hdr, const char *src, unsigned int size, rrid_t &rc)
{
	srv nu, *tmp;
	char host[256];

	if (!hdr.get(src, size, nu.priority))
		return false;
	if (!hdr.get(src, size, nu.weight))
		return false;
	if (!hdr.get(src, size, nu.port))
		return false;
	if (!hdr.get(src, size, host, sizeof(host)))
		return false;

	// RFC 2782 says that a "." in the host name means
	// that the service is unavailable.  Report so.
	if (host[0] == 0) {
		rc = REC_SRV_UNAVAIL;
		return false;
	}

	if ((tmp = new srv(nu)) == NULL)
		return false;

	tmp->next = get_srv();
	tmp->id = tmp->depth();

	if (rascal_isok(rascal_getaddr(host, on_getaddr, (void *)(unsigned int)((tmp->id << 16) | id)))) {
		get_srv() = tmp;
		debug((flog, rl_resolver, "%X: priority: %u, weight: %u, port: %u, name: %s, scheduled as: %u", get_id(), nu.priority, nu.weight, nu.port, host, tmp->id));
	} else {
		delete tmp;
		debug((flog, rl_resolver, "%X: request to resolve %s failed", get_id(), host));
	}

	return true;
}


void getsrv::on_event(header &hdr, const char *src, unsigned int size)
{
	rrid_t rc = REC_SUCCESS;

	debug((flog, rl_resolver, "%X: dispatching a getsrv event"));

	if (hdr.skip_echo(src, size)) {
		unsigned int _ttl;
		unsigned short _type, _class, _length;

		// Extracts responses.
		for (unsigned int idx = 0; idx < hdr.ancount && rascal_isok(rc); ++idx) {
			if (!hdr.skip_name(src, size))
				break;

			if (!hdr.get(src, size, _type))
				break;
			if (!hdr.get(src, size, _class))
				break;
			if (!hdr.get(src, size, _ttl))
				break;
			if (!hdr.get(src, size, _length))
				break;

			if (_length > size)
				break;

			if (_type == header::TYPE_SRV && _class == header::CLASS_IN && on_record(hdr, src, size, rc))
				break;

			src += _length;
			size -= _length;
		}
	}

	// No requests schedulled, resolution failed.
	if (get_srv() == NULL) {
		get_disp()(rascal_isok(rc) ? REC_SRV_NOTFOUND : rc, sock_t(), rop_connect, context);
		request::cancel();
	}
}


bool getsrv::try_next_server_disp(rrid_t rid, const sock_t *peer, int event, void *context)
{
	getsrv *req = reinterpret_cast<getsrv *>(request::pick((unsigned int)context));

	if (req == NULL) {
		debug((flog, rl_resolver, "%X: unknown context %p, rejecting (try_next_server_disp)", req->get_id(), context));
		return false;
	}

	if (event != rop_connect) {
		debug((flog, rl_resolver, "%X: bad event (%s) for %p, rejecting (try_next_server_disp)", req->get_id(), get_event_name(event), context));
		if (rascal_isok(rid))
			rascal_cancel(rid);
		return false;
	}

	if (rascal_isok(rid)) {
		bool rc;
		debug((flog, rl_resolver, "%X: connected to %s:%u, redispatching (try_next_server_disp)", req->get_id(), rascal::ntoa(peer->addr).c_str(), peer->port));
		rascal_set_dispatcher(rid, req->get_disp());
		rascal_set_context(rid, req->get_context());
		rc = req->get_disp()(rid, peer, event, req->get_context());
		delete req;
		return rc;
	}

	debug((flog, rl_resolver, "%X: connection failed, moving forward (try_next_server_disp)", req->get_id()));
	req->try_next_server();
	return false;
}


// Sorts the list according to the weight sorting rules defined in RFC2782.
// The list is expected to already have been sorted by priority, we only
// arrange it by element weight here.
void getsrv::sort()
{
	ina *nlist = NULL, **tail = &nlist;

	debug((flog, rl_resolver, "%X: sorting the list of A records", get_id()));

	while (get_ina() != NULL) {
		unsigned int rnd = 0;
		unsigned short priority = get_ina()->priority;

		// Overall weight of the current priority.
		for (ina *item = get_ina(); item != NULL && item->priority == priority; item = item->next)
			rnd += item->weight;

		// Find a random value.
		rnd = rnd ? faeutil_random(rnd - 1) : 0;

		// Find the first suitable element.
		for (ina **item = &get_ina(); *item != NULL && (*item)->priority == priority; rnd -= (*item)->weight, item = &(*item)->next) {
			if (rnd == 0 || rnd < (*item)->weight) {
				// Add to the new list.
				*tail = *item;
				// Remove from the old list.
				*item = (*item)->next;
				// End searching for the element.
				break;
			}
		}

		// We have not found anything suitable; unsure how this can
		// be, but to prevent us from entering an infinite loop, move
		// the current element to the new list.
		if (*tail == NULL) {
			// Insert to the new list.
			*tail = get_ina();
			// Remove from the old list.
			get_ina() = get_ina()->next;
		}

		// Skip the added element.
		if (*tail != NULL) {
			tail = &(*tail)->next;
			*tail = NULL;
		}
	}

	get_ina() = nlist;
}


void getsrv::try_next_server()
{
	if (get_ina() == NULL) {
		debug((flog, rl_resolver, "%X: reporting service unavailability (empty IN A list)", get_id()));
		get_disp()(REC_SRV_UNAVAIL, sock_t(), rop_connect, context);
		request::cancel();
	} else {
		bool try_next = true;
		ina *active = get_ina();

		get_ina() = active->next;

		if (get_filter() == NULL || get_filter()(context, &active->peer)) {
#if defined(_DEBUG)
			{
				char tmp[64];
				rascal_ntoa(&active->peer.addr, tmp, sizeof(tmp));
				debug((flog, rl_resolver, "%X: attempting to connect to %s:%u", get_id(), tmp, active->peer.port));
			}
#endif
			if (rascal_isok(rascal_connect(&active->peer, try_next_server_disp, (void*)(unsigned int)id))) {
				debug((flog, rl_resolver, "%X: connection schedulled, waiting", get_id()));
				try_next = false;
			} else {
				debug((flog, rl_resolver, "%X: connection rejected, moving to the next one", get_id()));
			}
		} else {
			debug((flog, rl_resolver, "%X: record %s:%u rejected by the filter", get_id(), rascal::ntoa(active->peer).c_str(), active->peer.port));
		}

		delete active;

		if (try_next)
			try_next_server();
	}
}


void getsrv::on_getaddr(unsigned int addrc, const addr_t *addrv, unsigned short id)
{
	for (srv **item = &get_srv(); *item != NULL; item = &(*item)->next) {
		if ((*item)->id == id) {
			srv *tmp = *item;

			for (unsigned int idx = 0; idx < addrc; ++idx) {
				ina *nu = new ina;
				if (nu != NULL) {
					nu->priority = tmp->priority;
					nu->weight = tmp->weight;
					nu->peer.port = tmp->port;
					nu->peer.addr = addrv[idx];

					for (ina **old = &get_ina(); ; old = &(*old)->next) {
						// Skip higher priorities.
						if (*old != NULL && (*old)->priority < nu->priority)
							continue;

						nu->next = *old;
						*old = nu;
						break;
					}
				}
			}

			*item = (*item)->next;
			delete tmp;
			break;
		}
	}

	if (get_srv() == NULL) {
		sort();
#if defined(_DEBUG)
		if (get_ina() != NULL) {
			unsigned int idx = 0, all = get_ina()->depth();
			for (ina *item = get_ina(); item != NULL; item = item->next) {
				char host[64];
				rascal_ntoa(&item->peer.addr, host, sizeof(host));
				debug((flog, rl_resolver, "%X:  ?? %02u/%02u. %s:%u, p=%u, w=%u\n", get_id(), ++idx, all, host, item->peer.port, item->priority, item->weight));
			}
		}
#endif
		try_next_server();
	}
}


void getsrv::on_getaddr(void *context, const char *, unsigned int addrc, const addr_t *addrv)
{
	unsigned short id = ((unsigned int)context) & 0xFFFF;
	request *req = request::pick(id);
	if (req != NULL) {
		reinterpret_cast<getsrv *>(req)->on_getaddr(addrc, addrv, ((unsigned int)context) >> 16);
	}
}
