// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(MQTTPERSISTENCEDEFAULT_H)
#define MQTTPERSISTENCEDEFAULT_H

/** Extension of the filename */
#define MESSAGE_FILENAME_EXTENSION ".msg"

/* prototypes of the functions for the default file system persistence */
int pstopen(void** handle, const char* clientID, const char* serverURI, void* context); 
int pstclose(void* handle); 
int pstput(void* handle, char* key, int bufcount, char* buffers[], int buflens[]); 
int pstget(void* handle, char* key, char** buffer, int* buflen); 
int pstremove(void* handle, char* key); 
int pstkeys(void* handle, char*** keys, int* nkeys); 
int pstclear(void* handle); 
int pstcontainskey(void* handle, char* key);

int pstmkdir(char *pPathname);

#endif

