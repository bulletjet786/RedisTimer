#ifndef REDISLEAGUE_T_DELAY_H
#define REDISLEAGUE_T_DELAY_H

#include "adlist.h"
#include "dict.h"

const uint32_t TIMER_NEAR_SHIFT = 8;
const uint32_t TIMER_NEAR_SIZE = 256;
const uint32_t TIMER_NEAR_MASK = TIMER_NEAR_SIZE - 1;
const uint32_t TIMER_LEVEL_SHIFT = 6;
const uint32_t TIMER_LEVEL_SIZE = 64;
const uint32_t TIMER_LEVEL_MASK = TIMER_LEVEL_SIZE - 1;
const uint32_t TIMER_LEVEL = 4;

typedef struct DelayQueueTypeObject DelayQueueTypeObject;
typedef struct TimeWheel TimeWheel;
typedef struct DelayMessageNode DelayMessageNode;

/* 表示一个延迟消息节点 */
typedef struct DelayMessage DelayMessage;
struct DelayMessage {
    char *id;
    char *body;
    uint32_t fire;
    list *location; // 在时间轮中的list
};
struct DelayMessageNode {
    list *list;
    listNode *node;
};
struct ChangeLog {

};

struct DelayQueueTypeObject {
    uint32_t size;                          // 整个延迟消息集中的数据量
    dict *dict;                             // Map<MessageID, *DelayMessageNode>

    list *prev;                             // List<id> 有序列表，保存比时间轮中小的数据
    list *near[TIMER_NEAR_SIZE];            // near保存在TIMER_NEAR_SIZE秒内到达的消息
    int nearBase;
    list *level[TIMER_LEVEL][TIMER_LEVEL_SIZE]; // level
    int levelBase[TIMER_LEVEL];             // 当前

};

// todo: DelayMessageFree
// todo: DelayMessageDup
// todo: DelayMessageMatch
// 如果未指定，则由list调用者进行删除，如果指定，则listDelNode()和listEmpty()将会调用该函数进行删除
void DelayMessageFree() {

};

// 如果未指定，则listDup将会拷贝value原始指针，如果指定了，则会使用该函数进行拷贝
void DelayMessageDup() {

};

// 如果未指定，则listMatch将会使用value原始指针，如果指定了，则会使用该函数进行搜索
void DelayMessageMatch() {

}


#endif //REDISLEAGUE_T_DELAY_H