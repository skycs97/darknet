#include "thpool_ex.h"
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <limits.h>
#include <utils.h>

#define _GNU_SOURCE

#define GPU
#define THREAD

double get_thread_min_time(threadpool thpool);

twin_thpool *twin_thpool_init(int thread_num_cpu, int thread_num_gpu)
{
    twin_thpool *twin_thpool_p = (twin_thpool *)malloc(sizeof(twin_thpool));

    if ((thread_num_cpu + thread_num_gpu) != 8)
    {
        err("thread nums sum is not 8");
        free(twin_thpool_p);
        return NULL;
    }

    twin_thpool_p->thpool_cpu = thpool_init(thread_num_cpu);
    twin_thpool_p->thpool_gpu = thpool_init(thread_num_gpu);

    int n = 0;
    int n_cpu = 0;
    int n_gpu = 0;
    cpu_set_t cpuset;

    for (n = 0; n < thread_num_cpu + thread_num_gpu; n++)
    {
        CPU_ZERO(&cpuset);
        CPU_SET(n, &cpuset);

        if (n_cpu < thread_num_cpu)
        {
            pthread_setaffinity_np(twin_thpool_p->thpool_cpu->threads[n_cpu], sizeof(cpu_set_t), cpuset);
            n_cpu++;
        }
        else if (n_gpu < thread_num_gpu)
        {
            pthread_setaffinity_np(twin_thpool_p->thpool_gpu->threads[n_gpu], sizeof(cpu_set_t), cpuset);
            n_gpu++;
        }
    }

    return twin_thpool_p;
}

int add_job(twin_thpool *twin_thpool_p, void (*function)(void *), netlayer *arg_p, int flag)
{
    threadpool cpu = twin_thpool_p->thpool_cpu;
    threadpool gpu = twin_thpool_p->thpool_gpu;

    double cpu_time = cpu->jobqueue.total_time;
    double gpu_time = gpu->jobqueue.total_time;

    int reflag = 0;

    if (gpu->jobqueue.total_time < cpu->jobqueue.total_time)
    {
        thpool_add_work(gpu, function_gpu, (void *)arg_p, arg_p->layer.exe_time_gpu);
        reflag = 1;
    }
    else
    {
        if (cpu_time + get_thread_min_time(cpu) < gpu_time + get_thread_min_time(gpu))
        {
            reflag = 0;
            thpool_add_work(cpu, function, (void *)arg_p, arg_p->layer.exe_time);
        }
        else
        {
            reflag = 1;
            thpool_add_work(gpu, function, (void *)arg_p, arg_p->layer.exe_time_gpu);
        }
    }

    return reflag;
}

double get_thread_min_time(threadpool thpool)
{
    int i = 0;

    double time = __DBL_MAX__;

    for (i = 0; i < thpool->thread_length; i++)
    {
        thread *thread_p = thpool->threads[i];

        double t = (thread_p->exe_time - (what_time_is_it_now() - thread_p->start_time));

        if (time > t)
            time = t;
    }

    return time;
}
