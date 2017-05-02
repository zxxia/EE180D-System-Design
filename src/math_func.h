#ifndef __MATH_FUNC_H__
#define __MATH_FUNC_H__
/* for fabsf() */
#include <math.h>

void max(double* data, int start_pos, int end_pos, double* max_val);
void min(double* data, int start_pos, int end_pos, double* min_val);
void mean(double* data, int start_pos, int end_pos, double* mean_val);
void variance(double* data, int start_pos, int end_pos, double* var);
void std(double* data, int start_pos, int end_pos, double* std_val);
void rms(double* data, int start_pos, int end_pos, double* rms_val);
void mad(double* data, int start_pos, int end_pos, double* mad_val);
void integral(double* data, double* time, int start_pos, int end_pos, double* integral_val);
#endif