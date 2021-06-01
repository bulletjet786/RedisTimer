#include "../include/redismodule.h"

int Hello_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModuleString *name = argv[1];
    const char *nameStr = RedisModule_StringPtrLen(name, NULL);
    RedisModuleString *helloReply = RedisModule_CreateStringPrintf(ctx, "hello, %s", nameStr);
    RedisModule_ReplyWithString(ctx, helloReply);
    RedisModule_FreeString(ctx, helloReply);
    return REDISMODULE_OK;
}

// HelloModule
int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx, "hello", 1, REDISMODULE_APIVER_1)
        == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "hello.hello",
                                  Hello_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    return REDISMODULE_OK;
}