#include "grim_fetcher.h"
#include <sys/time.h>

long get_delta_time(void)
{
    static long time;
    long        new_time;
    long        dt;

    new_time = get_time();
    if (!time)
        time = new_time;
    dt = new_time - time;
    time = new_time;
    return dt;
}

long get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
