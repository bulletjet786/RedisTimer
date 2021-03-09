#define REDISMODULE_EXPERIMENTAL_API

#include "../include/redismodule.h"
#include "adlist.h"
#include "utils.h"
#include "dict.h"

const char *DELAY_VERSION = "v0.1";

typedef struct DelayModule DelayModule;
struct DelayModule {
    uint32_t tickerTime;
    dict *keys;         // key name

};
static DelayModule *dm;

// 初始化DelayModule
void InitDelayModule(RedisModuleCtx *ctx, DelayModule *dm) {
    dm->tickerTime = getTimestamp();

    // 注册定时器
    RedisModule_CreateTimer();

}


int DingerVersion_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    REDISMODULE_NOT_USED(argv);
    REDISMODULE_NOT_USED(argc);
    RedisModule_ReplyWithSimpleString(ctx, DELAY_VERSION);
    return REDISMODULE_OK;
}

int DingerAdd_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModuleString *key = argv[1];

    RedisModule_OpenKey(ctx, key, REDISMODULE_WRITE);    // todo:


    const char *nameStr = RedisModule_StringPtrLen(name, NULL);
    RedisModuleString *helloReply = RedisModule_CreateStringPrintf(ctx, "hello, %s", nameStr);
    RedisModule_ReplyWithString(ctx, helloReply);
    RedisModule_FreeString(ctx, helloReply);
    return REDISMODULE_OK;
}

int DingerBfetch_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModuleString *name = argv[1];
    const char *nameStr = RedisModule_StringPtrLen(name, NULL);
    RedisModuleString *helloReply = RedisModule_CreateStringPrintf(ctx, "hello, %s", nameStr);
    RedisModule_ReplyWithString(ctx, helloReply);
    RedisModule_FreeString(ctx, helloReply);
    return REDISMODULE_OK;
}

int DingerFetch_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (argc != 2) return RedisModule_WrongArity(ctx);
    RedisModuleString *name = argv[1];
    const char *nameStr = RedisModule_StringPtrLen(name, NULL);
    RedisModuleString *helloReply = RedisModule_CreateStringPrintf(ctx, "hello, %s", nameStr);
    RedisModule_ReplyWithString(ctx, helloReply);
    RedisModule_FreeString(ctx, helloReply);
    return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    REDISMODULE_NOT_USED(argv);
    REDISMODULE_NOT_USED(argc);

    if (RedisModule_Init(ctx, "fire", 1, REDISMODULE_APIVER_1)
        == REDISMODULE_ERR)
        return REDISMODULE_ERR;
/*
    // 注册DelayMessage类型
    if (RedisModule_CreateDataType()) {
        return REDISMODULE_ERR;
    }
*/
    dm = RedisModule_Alloc(sizeof(DelayModule));
    // 初始化DelayModule模块
    InitDelayModule(ctx, dm);

    // 装载version命令
    if (RedisModule_CreateCommand(ctx, "delay.version",
                                  DingerVersion_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    // 装载add命令 todo: flag
    if (RedisModule_CreateCommand(ctx, "delay.add",
                                  DingerAdd_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    // 装载set命令 todo: flag
    if (RedisModule_CreateCommand(ctx, "delay.add",
                                  DingerAdd_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    // 装载bfetch命令 todo: flag
    if (RedisModule_CreateCommand(ctx, "delay.bfetch",
                                  DingerBfetch_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;

    // 装载fetch命令 todo: flag
    if (RedisModule_CreateCommand(ctx, "delay.fetch",
                                  DingerFetch_RedisCommand, "readonly", 1, 1, 1) == REDISMODULE_ERR)
        return REDISMODULE_ERR;


    return REDISMODULE_OK;
}



