#ifndef TIME_CHECKER_H
#define TIME_CHECKER_H

float convTime(int c, int size, int out_w, int out_h, int out_c);
float convTime_gpu(int c, int size, int out_w, int out_h, int out_c);
float avgTime(int w, int h, int c);
float avgTime_gpu(int w, int h, int c);
float connectTime(int inputs, int outputs);
float connectTime_gpu(int inputs, int outputs);
float maxTime(int size, int out_w, int out_h, int out_c);
float maxTime_gpu(int size, int out_w, int out_h, int out_c);
float routeTime(int input1, int input2);
float routeTime_gpu(int intput1, int input2);
float shortTime(int w1, int w2, int h1, int h2, int c1, int c2);
float shortTime_gpu(int w1, int w2, int h1, int h2, int c1, int c2);
float softTime(int inputs);
float softTime_gpu(int inputs);

#endif