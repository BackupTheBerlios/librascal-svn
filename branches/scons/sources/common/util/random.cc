// Faerion RunTime Library.
// Copyright (c) 2002-2004 hex@faerion.oss and others.
// $Id$

#include <stdlib.h>
#include <time.h>
#include "random.h"

namespace funs {

void faeutil_srandom(void)
{
#ifdef HAVE_arc4random
	arc4random_stir();
#else
	srand(time(NULL));
#endif
}


unsigned int faeutil_random(unsigned int range)
{
#ifdef HAVE_arc4random
	return arc4random() % range;
#else
	double tmp = (double)rand() / (double)RAND_MAX;
	return (unsigned int)((double)range * tmp);
#endif
}

}
