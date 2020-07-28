#include "time_checker.h"

inline double convTime(int c, int size, int out_w, int out_h, int out_c)
{
    return c * size * size * out_w * out_h * out_c * 3.5371923e-10;
}
inline double convTime_gpu(int c, int size, int out_w, int out_h, int out_c)
{
    return c * size * size * out_w * out_h * out_c * 6.95084835e-12;
}
inline double avgTime(int w, int h, int c)
{
    return w * h * c * 2.54309896e-09;
}
inline double avgTime_gpu(int w, int h, int c)
{
    return w * h * c * 4.71640625e-10;
}
inline double connectTime(int inputs, int outputs)
{
    return inputs * outputs * 3.60864721e-10;
}
inline double connectTime_gpu(int inputs, int outputs)
{
    return inputs * outputs * 9.2350978e-13;
}
inline double maxTime(int size, int out_w, int out_h, int out_c)
{
    return size * size * out_w * out_h * out_c * 3.7281712e-09;
}
inline double maxTime_gpu(int size, int out_w, int out_h, int out_c)
{
    return size * size * out_w * out_h * out_c * 1.1271307e-11;
}
inline double routeTime(int input1, int input2)
{
    return (double)(input1 * -1.21428506e-10) + (double)(input2 * 6.84337887e-09);
}
inline double routeTime_gpu(int input1, int input2)
{
    return (double)(input1 * -1.13869539e-10) + (double)(input2 * 7.35822402e-11);
}

inline double shortTime(int w1, int w2, int h1, int h2, int c1, int c2)
{
    int minw = (w1 > w2) ? w2 : w1;
    int minh = (h1 > h2) ? h2 : h1;
    int minc = (c1 > c2) ? c2 : c1;

    return minw * minh * minc * 1.3666221e-08;
}
inline double shortTime_gpu(int w1, int w2, int h1, int h2, int c1, int c2)
{
    int minw = (w1 > w2) ? w2 : w1;
    int minh = (h1 > h2) ? h2 : h1;
    int minc = (c1 > c2) ? c2 : c1;

    return minw * minh * minc * 8.17084917e-11;
}
inline double softTime(int inputs)
{
    return inputs * 6.424e-08;
}
inline double softTime_gpu(int inputs)
{
    return inputs * 7.07866667e-08;
}
