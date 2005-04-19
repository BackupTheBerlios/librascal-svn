// RASCAL: Realtime Asynchronous Connection Abstraction Layer.
// Copyright (c) 2003-2004 hex@faerion.oss
// Distributed under the terms of GNU LGPL, read 'LICENSE'.
//
// $Id$

#ifndef __rascal_nt_ntevent_h
#define __rascal_nt_ntevent_h

#ifndef QS_ALLPOSTMESSAGE
# define QS_ALLPOSTMESSAGE 0x100
#endif

class ntevent
{
	HANDLE hEvent;
public:
	ntevent() { hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); }
	~ntevent() { close(); }
	void close() { if (hEvent != NULL) { CloseHandle(hEvent); hEvent = NULL; } }
	void set() { SetEvent(hEvent); }
	operator HANDLE () { return hEvent; }
	WSAEVENT get_wsa() const { return reinterpret_cast<WSAEVENT>(hEvent); }
	DWORD wait_for_msg() { return MsgWaitForMultipleObjectsEx(1, &hEvent, INFINITE, QS_ALLPOSTMESSAGE, 2 /* MWMO_ALERTABLE */); }
};

#endif // __rascal_nt_ntevent_h
