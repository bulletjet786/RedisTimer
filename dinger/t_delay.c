#include "t_delay.h"
#include "../include/redismodule.h"
#include "utils.h"

list *computerTimeWheelBucket(DelayQueueTypeObject *dto, uint32_t delay, uint32_t fire) {
    // 如果可以在near时间内到达，则直接加入near链表
    if (delay < TIMER_NEAR_SIZE) {
        return dto->near[delay];
    }
    // 否则查找合适的level
    for (int i = 0; i < TIMER_LEVEL; i++) {
        uint32_t size = (uint32_t) 1 << (TIMER_NEAR_SHIFT + (TIMER_LEVEL_SHIFT * (i + 1)));
        if (fire < size) {
            uint32_t index = delay >> (TIMER_NEAR_SHIFT + (TIMER_LEVEL_SHIFT * i)) & TIMER_LEVEL_MASK;
            return dto->level[i][index];
        }
    }
    return NULL;
}

// 如果是新增返回1，如果是替换，返回0
int dtoSetDelayMessage(RedisModuleCtx *ctx, DelayQueueTypeObject *dto, char *id, char *message, int32_t delay) {
    int added = 1;
    uint32_t current = getTimestamp();

    DelayMessage *dm = RedisModule_Alloc(sizeof(struct DelayMessage));
    dm->id = id;
    dm->body = message;
    dm->fire = current + delay;
    char *newId = RedisModule_Strdup(dm->id);

    // 如果当前数据已经存在，则暂不支持 todo:
    dictEntry *existNode = dictFind(dto->dict, dm->id);
    if (existNode) {  // todo: dictCompareKeys
        DelayMessageNode *dmNode = (DelayMessageNode *) (existNode->v.val);
        listDelNode(dmNode->list, dmNode->node);
        dictDelete(dto->dict, dm->id);
        RedisModule_Free(dmNode->node);
        RedisModule_Free(dmNode);
        added = 0;
    }

    // 如果当前数据不存在
    if (!existNode) {
        // 如果比时间轮的时间小，则插入prev有序列表
        list *insertedList;
        if (delay < current) {
            // 如果list为空，则直接插入
            if (dto->prev->len == 0) {
                listAddNodeHead(dto->prev, newId);
            } else {
                listIter iter;
                listNode *node;

                // 查找到第一个比当前元素大的节点，插入到该位置前面
                listRewindTail(dto->prev, &iter);
                while ((node = listNext(&iter)) != NULL) {
                    DelayMessage *iterDm = (DelayMessage *) node->value;
                    if (iterDm->fire > dm->fire) {
                        continue;
                    }
                }
                listInsertNode(dto->prev, node, newId, 1);
            }
        } else {    // 如果比时间轮的时间大，则插入到timeWheel中
            insertedList = computerTimeWheelBucket(dto, delay, dm->fire);
            if (insertedList == NULL) {
                return 0;
            }
            // 插入到链表尾部
            listAddNodeTail(insertedList, newId);
        }
        dictAdd(dto->dict, dm->id, dm);
        dto->size += added;
    }

    return added;
}

void steal(RedisModuleCtx *ctx, DelayQueueTypeObject *dto) {
    // 不需要从level层进行偷取
    if (dto->nearBase != 0) {
        return;
    }

    // 计算需要偷取的层数
    int levelStealTimes;
    for (int i = 0; i < TIMER_LEVEL; i++) {
        if (dto->levelBase[i] == 0 && (dto->level[i][dto->levelBase[i]])->len == 0) {
            levelStealTimes++;
        }
    }

    uint32_t current = getTimestamp();
    // 从level高层，向level下层推送数据
    if (levelStealTimes != 0) {
        int stealLevel = levelStealTimes;
        for (int i = stealLevel; i > 0; i--) {
            // 推送一层
            listIter iter;
            listNode *node;

            // 查找到第一个比当前元素大的节点，插入到该位置前面
            list *from = dto->level[i][dto->levelBase];
            listRewind(from, &iter);
            while ((node = listNext(&iter)) != NULL) {
                DelayMessage *iterDm = (DelayMessage *) node->value;
                uint32_t delay = iterDm->fire - current;
                computerTimeWheelBucket()
                // 计算要插入的leve节点
                list *to = dto->level[i-1][iterDm->fire];
                listDelNode(from, node);
                listAddNodeHead(dto, )
            }
            listIter

        }
    }

}

void nextSecond(RedisModuleCtx *ctx, DelayQueueTypeObject *dto) {
    // 如果当前near轮已经转完，则从level轮进行偷取
    if (dto->near[dto->nearBase]->len == 0) {
        // 如果level轮已经转完，则从更高级的level中偷取
        for (int i = (TIMER_LEVEL - 1); i >= 0; i--) {
            if (dto->level[i] == 0) {

            }
        }
        steal(ctx, dto, 0, 1);
        for (int i = 0; i < TIMER_LEVEL; i++) {
            if (dto->level) {

            }
        }
    }

}

// 尝试推动时间轮到以下两个进度之一：
// 1、如果存在已到达但是还未消费的消息，则推动到最早的消息处
// 2、如果不存在已到达的消息，则推动到当前时间处
void dtoTryPushTimeWheel(RedisModuleCtx *ctx, DelayQueueTypeObject *dto) {
    uint32_t current = getTimestamp();

    // 一直推进，直到推进到当前时间，如果存在已到达但是未消费的消息处，则停止推进
    int32_t diff = current - (dto->baseTime + dto->nearBase);
    while (diff > 0) {
        nextSecond(ctx, dto);
        diff = current - (dto->baseTime + dto->nearBase);
    }

    if (dto->near[dto->nearBase]->len == 0) {

    }
}

// 从延迟消息中弹出一个到达的消息
DelayMessage *dtoPopDelayMessage(RedisModuleCtx *ctx, DelayQueueTypeObject *dto, DelayMessage *dm) {
    if (dto->size == 0) {
        return NULL;
    }

    // 检查prev是否有消息
    DelayMessage *ret;
    if (dto->prev->len != 0) {
        listNode *node = listFirst(dto->prev);
        ret = (DelayMessage *) node->value;
        listDelNode(dto->prev, node);
        dictDelete(dto->dict, ret->id);
        return ret;
    }

    // 尝试从时间轮中弹出一个消息，此时时间轮必然有数据
    if ()


}

// 如果创建失败，返回NULL
DelayQueueTypeObject *CreateDelayTypeObject() {
    DelayQueueTypeObject *dto;

    if ((dto = RedisModule_Alloc(sizeof(*dto))) == NULL)
        return NULL;

    dto->prev = listCreate();

    for (int i = 0; i < TIMER_NEAR_SIZE; ++i) {
        dto->near[i] = listCreate();
    }
    for (int i = 0; i < TIMER_LEVEL; ++i) {
        for (int j = 0; j < TIMER_LEVEL_SIZE; ++j) {
            dto->level[i][j] = listCreate();
        }
    }

    return dto;
}


// 向该时间轮中插入一条消息
// 前置条件：该消息的delay应不小于时间轮的startPoint，如果小于，则插入失败并返回0，否则返回1
int TwAddDelayMessage(TimeWheel *tw, DelayMessage *dm, list **insertedList, listNode **insertedNode) {
    list *bucket = computerTimeWheelBucket(tw, dm->fire);
    if (bucket == NULL) {
        return 0;
    }

    // 插入到链表尾部
    listNode *node;
    listAddNodeTail(bucket, dm);
    node = listLast(bucket);
    if (insertedList != NULL) {
        insertedList = &bucket;
    }
    if (insertedNode != NULL) {
        insertedNode = &node;
    }
    return 1;
}

DelayMessage *TwPopDelayMessage(TimeWheel *tw) {

}

void delDelayMessage(TimeWheel *tw, char *messageId) {

}

// 时间轮转动一次
void next() {

}