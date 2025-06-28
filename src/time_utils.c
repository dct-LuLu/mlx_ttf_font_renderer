#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "error_handler.h"

/*
        Function that computes the time based on EPOCH
*/
ssize_t get_current_time(void)
{
        struct timeval  time;

        if (gettimeofday(&time, NULL) == -1)
                return (error(ERR_TIME, NULL), -1);
        return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}