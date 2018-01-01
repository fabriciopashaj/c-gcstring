#include "lib.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#ifndef SIZE
#define SIZE 1024
#endif

typedef struct Bucket {
	size_t   refcount;
	uint32_t hash;
	String   str;
} Bucket;

static Bucket memory[SIZE];
static uint32_t count = 0;
bool initialised = false;

static inline __attribute__((always_inline))
void init()
{
	memset(memory, 0, sizeof(memory));
	initialised = true;
}
void StringGC_run()
{
	Bucket *bucket;
	size_t i = 0;
	while (i < SIZE)
	{
		bucket = &memory[i];
		if (bucket->str != NULL && bucket->refcount == 0)
		{
			String_cleanup(bucket->str);
			bucket->hash = 0;
			bucket->str = NULL;
			count--;
		}
		i++;
	}
}
GCString StringGC_register(String str)
{
	if (!initialised) init();

	uint32_t hash = String_hash(str);
	size_t index = hash % SIZE;
	Bucket *bucket;
	do {
		if (index >= SIZE)
			return NULL;
		bucket = &memory[index++];
	} while (bucket->str != NULL && bucket->hash != hash &&
			 !String_equal(str, bucket->str));
	if (bucket->str == NULL)
	{
	bucket->str = str;
	bucket->hash = hash;
	}
	GCString_use(&bucket->str);
	return &bucket->str;
}
size_t GCString_use(GCString str)
{
	if (count / 2 * 3 >= SIZE)
		StringGC_run();
	Bucket *bucket = (void *)str - offsetof(Bucket, str);
	size_t current_refcount = bucket->refcount;
	if (++(bucket->refcount) < current_refcount)
	{
		perror("String reference count overflow");
		exit(ERANGE);
	}
	count++;
	return current_refcount;
}
size_t GCString_dispose(GCString str)
{
	if (count == 0)
		return 0;
	Bucket *bucket = (void *)str - offsetof(Bucket, str);
	return (bucket->refcount == 0) ? 0 : (bucket->refcount)--;
}
