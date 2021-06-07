#define TEST

#ifdef TEST

#include <malloc.h>
#include <strings.h>
#define tmalloc(size) (malloc(size))
#define trealloc(ptr, size) (realloc(ptr, size))
#define tfree(ptr) (free(ptr))
#define tcalloc(b, size) (calloc(b, size))
#define tstrdup(str) (strdup)(str))

#else

#include "../include/redismodule.h"
#define tmalloc(size) (RedisModule_Alloc(size))
#define trealloc(ptr, size) (RedisModule_Realloc(ptr, size))
#define tfree(ptr) (RedisModule_Free(ptr))
#define tcalloc(b, size) (RedisModule_Calloc(b, size))
#define tstrdup(str) (RedisModule_Strdup)(str))

#endif