#ifndef TIME_CHECKER_H
#define TIME_CHECKER_H

inline double convTime(int c, int size, int out_w, int out_h, int out_c);
inline double convTime_gpu(int c, int size, int out_w, int out_h, int out_c);
inline double avgTime(int w, int h, int c);
inline double avgTime_gpu(int w, int h, int c);
inline double connectTime(int inputs, int outputs);
inline double connectTime_gpu(int inputs, int outputs);
inline double maxTime(int size, int out_w, int out_h, int out_c);
inline double maxTime_gpu(int size, int out_w, int out_h, int out_c);
inline double routeTime(int input1, int input2);
inline double routeTime_gpu(int intput1, int input2);
inline double shortTime(int w1, int w2, int h1, int h2, int c1, int c2);
inline double shortTime_gpu(int w1, int w2, int h1, int h2, int c1, int c2);
inline double softTime(int inputs);
inline double softTime_gpu(int inputs);

#endif