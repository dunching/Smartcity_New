// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(THREAD_H)
#define THREAD_H

#if !defined(_WIN32) && !defined(_WIN64)
#if defined(__GNUC__) && defined(__linux__)
#if !defined(_GNU_SOURCE)
// for pthread_setname
	#define _GNU_SOURCE
#endif
#endif
#endif

#include "MQTTExportDeclarations.h"

#include "MQTTClient.h"

#include "mutex_type.h" /* Needed for mutex_type */

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
	#define thread_type HANDLE
	#define thread_id_type DWORD
	#define thread_return_type DWORD
	#define thread_fn LPTHREAD_START_ROUTINE
	#define cond_type HANDLE
	#define sem_type HANDLE
	#undef ETIMEDOUT
	#define ETIMEDOUT WSAETIMEDOUT
#else
	#include <pthread.h>

	#define thread_type pthread_t
	#define thread_id_type pthread_t
	#define thread_return_type void*
	typedef thread_return_type (*thread_fn)(void*);
	typedef struct { pthread_cond_t cond; pthread_mutex_t mutex; } cond_type_struct;
	typedef cond_type_struct *cond_type;
	#if defined(OSX)
	  #include <dispatch/dispatch.h>
	  typedef dispatch_semaphore_t sem_type;
	#else
	  #include <semaphore.h>
	  typedef sem_t *sem_type;
	#endif

	cond_type Thread_create_cond(int*);
	int Thread_signal_cond(cond_type);
	int Thread_wait_cond(cond_type condvar, int timeout);
	int Thread_destroy_cond(cond_type);
#endif

LIBMQTT_API void Thread_start(thread_fn, void*);
int Thread_set_name(const char* thread_name);

LIBMQTT_API mutex_type Thread_create_mutex(int*);
LIBMQTT_API int Thread_lock_mutex(mutex_type);
LIBMQTT_API int Thread_unlock_mutex(mutex_type);
int Thread_destroy_mutex(mutex_type);

LIBMQTT_API thread_id_type Thread_getid();

sem_type Thread_create_sem(int*);
int Thread_wait_sem(sem_type sem, int timeout);
int Thread_check_sem(sem_type sem);
int Thread_post_sem(sem_type sem);
int Thread_destroy_sem(sem_type sem);


#endif
