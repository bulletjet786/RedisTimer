#ifndef REDISLEAGUE_T_DELAY_H
#define REDISLEAGUE_T_DELAY_H

#include "adlist.h"
#include "dict.h"
#include <stdlib.h>
#include <string.h>

const uint32_t TIMER_NEAR_SHIFT = 8;
const uint32_t TIMER_NEAR_SIZE = 256;
const uint32_t TIMER_NEAR_MASK = TIMER_NEAR_SIZE - 1;
const uint32_t TIMER_LEVEL_SHIFT = 6;
const uint32_t TIMER_LEVEL_SIZE = 64;
const uint32_t TIMER_LEVEL_MASK = TIMER_LEVEL_SIZE - 1;
const uint32_t TIMER_LEVEL = 4;

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

// todo: DelayMessageFree
// todo: DelayMessageDup
// todo: DelayMessageMatch
// 如果未指定，则由list调用者进行删除，如果指定，则listDelNode()和listEmpty()将会调用该函数进行删除
int TimerListMatch(void *ptr, void *key) {
    return strcmp(ptr, key);
};

// 如果未指定，则listDup将会拷贝value原始指针，如果指定了，则会使用该函数进行拷贝
int TimerDictKeyCompare(void *privdata, const void *key1, const void *key2) {
    return strcmp(key1, key2);
};


#endif //REDISLEAGUE_T_DELAY_H