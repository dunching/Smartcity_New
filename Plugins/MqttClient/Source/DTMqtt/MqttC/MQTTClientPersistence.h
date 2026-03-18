// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(MQTTCLIENTPERSISTENCE_H)
#define MQTTCLIENTPERSISTENCE_H
/*
/// @endcond
*/

/**
  * This <i>persistence_type</i> value specifies the default file system-based 
  * persistence mechanism (see MQTTClient_create()).
  */
#define MQTTCLIENT_PERSISTENCE_DEFAULT 0
/**
  * This <i>persistence_type</i> value specifies a memory-based 
  * persistence mechanism (see MQTTClient_create()).
  */
#define MQTTCLIENT_PERSISTENCE_NONE 1
/**
  * This <i>persistence_type</i> value specifies an application-specific 
  * persistence mechanism (see MQTTClient_create()).
  */
#define MQTTCLIENT_PERSISTENCE_USER 2

/** 
  * Application-specific persistence functions must return this error code if 
  * there is a problem executing the function. 
  */
#define MQTTCLIENT_PERSISTENCE_ERROR -2

/**
  * @brief Initialize the persistent store.
  * 
  * Either open the existing persistent store for this client ID or create a new
  * one if one doesn't exist. If the persistent store is already open, return 
  * without taking any action.
  *
  * An application can use the same client identifier to connect to many
  * different servers. The <i>clientid</i> in conjunction with the 
  * <i>serverURI</i> uniquely identifies the persistence store required.
  *
  * @param handle The address of a pointer to a handle for this persistence 
  * implementation. This function must set handle to a valid reference to the 
  * persistence following a successful return. 
  * The handle pointer is passed as an argument to all the other
  * persistence functions. It may include the context parameter and/or any other
  * data for use by the persistence functions.
  * @param clientID The client identifier for which the persistent store should 
  * be opened.
  * @param serverURI The connection string specified when the MQTT client was
  * created (see MQTTClient_create()).
  * @param context A pointer to any data required to initialize the persistent
  * store (see ::MQTTClient_persistence).
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_open)(void** handle, const char* clientID, const char* serverURI, void* context);

/**
  * @brief Close the persistent store referred to by the handle.
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_close)(void* handle); 

/**
  * @brief Put the specified data into the persistent store.
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @param key A string used as the key for the data to be put in the store. The
  * key is later used to retrieve data from the store with Persistence_get().
  * @param bufcount The number of buffers to write to the persistence store.
  * @param buffers An array of pointers to the data buffers associated with 
  * this <i>key</i>.
  * @param buflens An array of lengths of the data buffers. <i>buflen[n]</i> 
  * gives the length of <i>buffer[n]</i>.
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_put)(void* handle, char* key, int bufcount, char* buffers[], int buflens[]);

/**
  * @brief Retrieve the specified data from the persistent store. 
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @param key A string that is the key for the data to be retrieved. This is 
  * the same key used to save the data to the store with Persistence_put().
  * @param buffer The address of a pointer to a buffer. This function sets the
  * pointer to point at the retrieved data, if successful.
  * @param buflen The address of an int that is set to the length of 
  * <i>buffer</i> by this function if successful.
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_get)(void* handle, char* key, char** buffer, int* buflen);

/**
  * @brief Remove the data for the specified key from the store.
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @param key A string that is the key for the data to be removed from the
  * store. This is the same key used to save the data to the store with 
  * Persistence_put().
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_remove)(void* handle, char* key);

/**
  * @brief Returns the keys in this persistent data store.
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @param keys The address of a pointer to pointers to strings. Assuming
  * successful execution, this function allocates memory to hold the returned
  * keys (strings used to store the data with Persistence_put()). It also 
  * allocates memory to hold an array of pointers to these strings. <i>keys</i>
  * is set to point to the array of pointers to strings.
  * @param nkeys A pointer to the number of keys in this persistent data store. 
  * This function sets the number of keys, if successful.
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_keys)(void* handle, char*** keys, int* nkeys);

/**
  * @brief Clears the persistence store, so that it no longer contains any 
  * persisted data.
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @return Return 0 if the function completes successfully, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_clear)(void* handle);

/**
  * @brief Returns whether any data has been persisted using the specified key.
  *
  * @param handle The handle pointer from a successful call to 
  * Persistence_open().
  * @param key The string to be tested for existence in the store.
  * @return Return 0 if the key was found in the store, otherwise return
  * ::MQTTCLIENT_PERSISTENCE_ERROR.
  */
typedef int (*Persistence_containskey)(void* handle, char* key);

/**
  * @brief A structure containing the function pointers to a persistence 
  * implementation and the context or state that will be shared across all 
  * the persistence functions.
  */
typedef struct {
  /** 
    * A pointer to any data required to initialize the persistent store.
    */
	void* context;
  /** 
    * A function pointer to an implementation of Persistence_open().
    */
	Persistence_open popen;
  /** 
    * A function pointer to an implementation of Persistence_close().
    */
	Persistence_close pclose;
  /**
    * A function pointer to an implementation of Persistence_put().
    */
	Persistence_put pput;
  /** 
    * A function pointer to an implementation of Persistence_get().
    */
	Persistence_get pget;
  /** 
    * A function pointer to an implementation of Persistence_remove().
    */
	Persistence_remove premove;
  /** 
    * A function pointer to an implementation of Persistence_keys().
    */
	Persistence_keys pkeys;
  /** 
    * A function pointer to an implementation of Persistence_clear().
    */
	Persistence_clear pclear;
  /** 
    * A function pointer to an implementation of Persistence_containskey().
    */
	Persistence_containskey pcontainskey;
} MQTTClient_persistence;


/**
 * A callback which is invoked just before a write to persistence.  This can be
 * used to transform the data, for instance to encrypt it.
 * @param context The context as set in ::MQTTAsync_setBeforePersistenceWrite
 * @param bufcount The number of buffers to write to the persistence store.
 * @param buffers An array of pointers to the data buffers.
 * @param buflens An array of lengths of the data buffers.
 * @return Return 0 if the function completes successfully, otherwise non 0.
 */
typedef int MQTTPersistence_beforeWrite(void* context, int bufcount, char* buffers[], int buflens[]);


/**
 * A callback which is invoked just after a read from persistence.  This can be
 * used to transform the data, for instance to decrypt it.
 * @param context The context as set in ::MQTTAsync_setAfterPersistenceRead
 * @param buffer The address of a pointer to a buffer.
 * @param buflen The address of an int that is the length of the buffer.
 * @return Return 0 if the function completes successfully, otherwise non 0.
 */
typedef int MQTTPersistence_afterRead(void* context, char** buffer, int* buflen);

#endif
