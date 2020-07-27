#define _GNU_SOURCE
#include <sched.h>

#include <pthread.h>
#include "thpool.h"
#include "thpool_ex.h"
#include "cuda.h"

#include <stdio.h>

#include <limits.h>
#include <utils.h>

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
            pthread_setaffinity_np(twin_thpool_p->thpool_cpu->threads[n_cpu], sizeof(cpu_set_t), &cpuset);
            n_cpu++;
        }
        else if (n_gpu < thread_num_gpu)
        {
            pthread_setaffinity_np(twin_thpool_p->thpool_gpu->threads[n_gpu], sizeof(cpu_set_t), &cpuset);
            n_gpu++;
        }
    }

    return twin_thpool_p;
}

int add_job(twin_thpool *twin_thpool_p, void (*function)(void *), netlayer *arg_p, int flag, int *routeOrShort)
{
    threadpool cpu = twin_thpool_p->thpool_cpu;
    threadpool gpu = twin_thpool_p->thpool_gpu;

    double cpu_time = cpu->jobqueue.total_time;
    double gpu_time = gpu->jobqueue.total_time;

    int a = 0;
    if (gpu->jobqueue.total_time <= cpu->jobqueue.total_time)
    {
        arg_p->flag = 1;
    }
    else
    {
        if (cpu_time + get_thread_min_time(cpu) < gpu_time + get_thread_min_time(gpu))
        {
            arg_p->flag = 0;
        }
        else
        {
            arg_p->flag = 1;
        }
    }

    // if (arg_p->layer.type == SHORTCUT)
    // {
    //     int idx = arg_p->layer.index;

    //     layer preLayer = arg_p->net.layers[idx];

    //     if ((arg_p->flag == 0) && (routeOrShort[idx] == 1))
    //     {
    //         cuda_pull_array(preLayer.output_gpu, preLayer.output, preLayer.outputs);
    //     }
    //     else if ((arg_p->flag == 1) && (routeOrShort[idx] == 0))
    //     {
    //         cuda_push_array(preLayer.output_gpu, preLayer.output, preLayer.outputs);
    //     }
    // }
    // else if (arg_p->layer.type == ROUTE)
    // {
    //     int idx_1 = arg_p->layer.input_layers[0];
    //     int idx_2 = arg_p->layer.input_layers[1];
    //     layer preLayer = arg_p->net.layers[idx_1];
    //     layer preLayer2 = arg_p->net.layers[idx_2];

    //     if ((arg_p->flag == 0))
    //     {
    //         if (routeOrShort[idx_1] == 1)
    //             cuda_pull_array(preLayer.output_gpu, preLayer.output, preLayer.outputs);
    //         if (routeOrShort[idx_2] == 1)
    //             cuda_pull_array(preLayer2.output_gpu, preLayer2.output, preLayer2.outputs);
    //     }
    //     else if ((arg_p->flag == 1))
    //     {
    //         if (routeOrShort[idx_1] == 0)
    //             cuda_push_array(preLayer.output_gpu, preLayer.output, preLayer.outputs);
    //         if (routeOrShort[idx_2] == 0)
    //             cuda_push_array(preLayer2.output_gpu, preLayer2.output, preLayer2.outputs);
    //     }
    // }

    if (arg_p->flag == 0)
    {
        if (flag == 1)
        {
            //cuda_pull_array(arg_p->net.input_gpu, arg_p->net.input, arg_p->net.inputs * arg_p->net.batch);
            //printf("gpu->cpu - %d\n", arg_p->net.index_n);
        }
        thpool_add_work(cpu, function, (void *)arg_p, arg_p->layer.exe_time);
    }
    else if (arg_p->flag == 1)
    {
        if (flag == 0)
        {
            //cuda_push_array(arg_p->net.input_gpu, arg_p->net.input, ((arg_p->net).inputs) * ((arg_p->net).batch));
            //printf("cpu->gpu - %d\n", arg_p->net.index_n);
        }
        thpool_add_work(gpu, function, (void *)arg_p, arg_p->layer.exe_time_gpu);
    }

    return arg_p->flag;
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
