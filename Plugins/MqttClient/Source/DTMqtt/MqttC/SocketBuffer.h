// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(SOCKETBUFFER_H)
#define SOCKETBUFFER_H

#include "Socket.h"

#if defined(OPENSSL)
#include <openssl/ssl.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
	typedef WSABUF iobuf;
#else
	typedef struct iovec iobuf;
#endif

typedef struct
{
	SOCKET socket;
	unsigned int index;
	size_t headerlen;
	char fixed_header[5];	/**< header plus up to 4 length bytes */
	size_t buflen, 			/**< total length of the buffer */
		datalen; 			/**< current length of data in buf */
	char* buf;
} socket_queue;

typedef struct
{
	SOCKET socket;
	int count;
	size_t total;
#if defined(OPENSSL)
	SSL* ssl;
#endif
	size_t bytes;
	iobuf iovecs[5];
	int frees[5];
} pending_writes;

#define SOCKETBUFFER_COMPLETE 0
#if !defined(SOCKET_ERROR)
	#define SOCKET_ERROR -1
#endif
#define SOCKETBUFFER_INTERRUPTED -22 /* must be the same value as TCPSOCKET_INTERRUPTED */

int SocketBuffer_initialize(void);
void SocketBuffer_terminate(void);
void SocketBuffer_cleanup(SOCKET socket);
char* SocketBuffer_getQueuedData(SOCKET socket, size_t bytes, size_t* actual_len);
int SocketBuffer_getQueuedChar(SOCKET socket, char* c);
void SocketBuffer_interrupted(SOCKET socket, size_t actual_len);
char* SocketBuffer_complete(SOCKET socket);
void SocketBuffer_queueChar(SOCKET socket, char c);

#if defined(OPENSSL)
int SocketBuffer_pendingWrite(SOCKET socket, SSL* ssl, int count, iobuf* iovecs, int* frees, size_t total, size_t bytes);
#else
int SocketBuffer_pendingWrite(SOCKET socket, int count, iobuf* iovecs, int* frees, size_t total, size_t bytes);
#endif
pending_writes* SocketBuffer_getWrite(SOCKET socket);
int SocketBuffer_writeComplete(SOCKET socket);
pending_writes* SocketBuffer_updateWrite(SOCKET socket, char* topic, char* payload);

#endif
