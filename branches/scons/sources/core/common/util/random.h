/*
 * Faerion RunTime Library.
 * Copyright (c) 2003-2004 hex@faerion.oss and others.
 * Distributed under the terms of GNU LGPL, read 'LICENSE'.
 *
 * $Id$
 *
 */

#ifndef __librascal_common_util_random_h
#define __librascal_common_util_random_h

namespace funs {

void faeutil_srandom(void);
unsigned int faeutil_random(unsigned int);

}

using namespace funs;

#endif /* __librascal_common_util_random_h */
