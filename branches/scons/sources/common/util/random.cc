// Faerion RunTime Library.
// Copyright (c) 2002-2004 hex@faerion.oss and others.
// $Id$

#include <stdlib.h>
#if defined(HAVE_CryptGenRandom)
# include <windows.h>
# include <wincrypt.h>
#else
# include <time.h>
#endif
#include "random.h"

#if defined(HAVE_CryptGenRandom)
static HCRYPTPROV hCryptProv = NULL;
#endif

namespace funs {

void faeutil_srandom(void)
{
#if defined(HAVE_arc4random)
	arc4random_stir();
#elif defined(HAVE_CryptGenRandom)
	if (hCryptProv == NULL)
		CryptAcquireContext (&hprov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
#else
	srand(time(NULL));
#endif
}


unsigned int faeutil_random(unsigned int range)
{
#ifdef HAVE_arc4random
	return arc4random() % range;
#elif defined(HAVE_CryptGenRandom)
	unsigned int rnd = 0;
	if (hCryptProv != NULL)
		CryptGenRandom (hCryptProv, sizeof(rnd), &rnd);
	return rnd;
#else
	double tmp = (double)rand() / (double)RAND_MAX;
	return (unsigned int)((double)range * tmp);
#endif
}

}
