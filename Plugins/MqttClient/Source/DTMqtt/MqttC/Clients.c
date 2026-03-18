// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "Clients.h"

#include <string.h>
#include <stdio.h>

/**
 * List callback function for comparing clients by clientid
 * @param a first integer value
 * @param b second integer value
 * @return boolean indicating whether a and b are equal
 */
int clientIDCompare(void* a, void* b)
{
	Clients* client = (Clients*)a;
	/*printf("comparing clientdIDs %s with %s\n", client->clientID, (char*)b);*/
	return strcmp(client->clientID, (char*)b) == 0;
}


/**
 * List callback function for comparing clients by socket
 * @param a first integer value
 * @param b second integer value
 * @return boolean indicating whether a and b are equal
 */
int clientSocketCompare(void* a, void* b)
{
	Clients* client = (Clients*)a;
	/*printf("comparing %d with %d\n", (char*)a, (char*)b); */
	return client->net.socket == *(SOCKET*)b;
}
