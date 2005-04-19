// Faerion RunTime Library.
// Copyright (c) 2002-2004 hex@faerion.oss and others.
// $Id$

#include "fustring.h"

namespace funs {

void strlcpy_local(char *dst, const char *src, size_t size)
{
	while (size != 0 && *src != '\0') {
		*dst++ = *src++;
		--size;
	}

	if (size == 0)
		--dst;

	*dst = '\0';
}

}
