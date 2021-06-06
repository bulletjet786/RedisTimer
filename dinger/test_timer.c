#include "timer.h"
#include "utils.h"

#define TEST

int main() {
    Timer *timer = CreateTimer();
    char *taskId = "yanghailin";
    char *taskBody = "{\"name\": \"yanghailin\"}";
    uint32_t fire = getTimestamp() + 120;
    TimerSet(NULL, timer,taskId, taskBody, fire, 0, 0);
    printf("Done");
}