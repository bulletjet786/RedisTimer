#include <stdint.h>
#include <sys/time.h>

uint32_t getTimestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}