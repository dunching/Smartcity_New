// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(_MUTEX_TYPE_H_)
#define _MUTEX_TYPE_H_

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
	#define mutex_type HANDLE
#else
	#include <pthread.h>
	#define mutex_type pthread_mutex_t*
#endif

#endif /* _MUTEX_TYPE_H_ */
