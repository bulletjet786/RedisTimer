//
// Created by 85123 on 2021/4/30.
//

#ifndef REDISTIMER_ALLOC_H
#define REDISTIMER_ALLOC_H

#define zmalloc(size) RedisModule_Alloc(size)
#define zcalloc(count, size) RedisModule_Calloc(count, size)
#define zrealloc(ptr, size) RedisModule_Realloc(ptr, size)
#define zfree(ptr) RedisModule_Free(ptr)
#define zstrdup(ptr) RedisModule_Strdup(ptr)

#endif //REDISTIMER_ALLOC_H
