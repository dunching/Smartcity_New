// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(OSWRAPPER_H)
#define OSWRAPPER_H

#if defined(_WRS_KERNEL)
#include <time.h>

#define lstat stat

typedef unsigned long useconds_t;
void usleep(useconds_t useconds);

#define timersub(a, b, result) \
	do \
	{ \
		(result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
		(result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
		if ((result)->tv_usec < 0) \
		{ \
			--(result)->tv_sec; \
			(result)->tv_usec += 1000000L; \
		} \
	} while (0)
#endif /* defined(_WRS_KERNEL) */

#endif /* OSWRAPPER_H */
