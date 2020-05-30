#ifndef THPOOL_EX_H
#define THPOOL_EX_H

#include "thpool.h"

typedef struct twin_thpool
{
    threadpool thpool_cpu;
    threadpool thpool_gpu;
} twin_thpool;

twin_thpool *twin_thpool_init(int, int);

int add_job(twin_thpool *, void (*function_p_cpu)(void *), void (*function_p_gpu)(void *), netlayer *arg_p);

#endif