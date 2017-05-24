#ifndef MATH_FUNC_H
#define MATH_FUNC_H
/* for fabsf() */
#include <math.h>

//Compute maximum in data from start_pos to end_pos. Store the result in max_val.
void max(double* data, int start_pos, int end_pos, double* max_val);

//Compute minimum in data from start_pos to end_pos. Store the result in min_val.
void min(double* data, int start_pos, int end_pos, double* min_val);

//Compute mean in data from start_pos to end_pos. Store the result in mean_val.
void mean(double* data, int start_pos, int end_pos, double* mean_val);

//Compute variance in data from start_pos to end_pos. Store the result in var.
void variance(double* data, int start_pos, int end_pos, double* var);

//Compute standard deviation in data from start_pos to end_pos. Store the result in std_val.
void std(double* data, int start_pos, int end_pos, double* std_val);

//Compute root-mean-square in data from start_pos to end_pos. Store the result in rms_val.
void rms(double* data, int start_pos, int end_pos, double* rms_val);

//Compute mean absolute deviation in data from start_pos to end_pos. Store the result in mad_val.
void mad(double* data, int start_pos, int end_pos, double* mad_val);

//Compute numerical intergral in data from start_pos to end_pos. Store the result in integral_val.
void integral(double* data, double* time, int start_pos, int end_pos, double* integral_val);
#endif
