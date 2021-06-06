#ifndef REDISLEAGUE_TIMER_H
#define REDISLEAGUE_TIMER_H

#include "adlist.h"
#include "dict.h"
#include <stdlib.h>
#include <string.h>
#include "../include/redismodule.h"

#define TIMER_NEAR_SHIFT 8
#define TIMER_NEAR_SIZE 256
#define TIMER_NEAR_MASK (TIMER_NEAR_SIZE - 1)
#define TIMER_LEVEL_SHIFT 6
#define TIMER_LEVEL_SIZE 64
#define TIMER_LEVEL_MASK (TIMER_LEVEL_SIZE - 1)
#define TIMER_LEVEL 4

typedef struct Timer Timer;
typedef struct TimeWheel TimeWheel;
typedef struct DelayMessageNode DelayMessageNode;

/* 表示一个延迟消息节点 */
typedef struct TimerTask TimerTask;
struct TimerTask {
    char *id;
    char *body;
    uint32_t fire;
    list *location; // 在时间轮中的list
};

struct Timer {
    uint32_t size;                          // 整个延迟消息集中的数据量
    dict *dict;                             // Map<char *, TimerTask *>

    list *prev;                             // List<char*> 有序列表，保存比时间轮基底中小的数据
    list *near[TIMER_NEAR_SIZE];            // near保存在TIMER_NEAR_SIZE秒内到达的消息，
    int nearBase;
    list *level[TIMER_LEVEL][TIMER_LEVEL_SIZE]; // level
    int levelBase[TIMER_LEVEL];             // 当前
    uint32_t timeWheelBase;                 // 时间轮的基底时间
};

int TimerSet(RedisModuleCtx *ctx, Timer *timer, char *id, char *body, uint32_t fire, int px,
             int nx);
Timer *CreateTimer();

#endif