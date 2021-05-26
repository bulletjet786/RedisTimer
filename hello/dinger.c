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

// TimerModule:
// timer.set key id data fire 设置一个定时器，该定时器将会在fire时到达，然后可以通过bpop拉取到
// timer.bpop key ms 监控一个定时器队列，当定时器到达时，该方法将会一直阻塞直到经过了ms
int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx, "timer", 1, REDISMODULE_APIVER_1)
        == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "timer.hello",
                                  Hello_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "timer.bpop",
            TmBPop_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    if (RedisModule_CreateCommand(ctx, "timer.set",
            TmSet_RedisCommand, "", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    return REDISMODULE_OK;
}