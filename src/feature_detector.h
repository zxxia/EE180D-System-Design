#ifndef __FEATURE_DETECTOR_H__
#define __FEATURE_DETECTOR_H__
/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
#include "math_func.h"


typedef struct
{
	double accel_y_seg0_max;
	double accel_y_seg0_min;
	double accel_y_seg1_max;
	double accel_y_seg1_min;
	double accel_y_seg2_max;
	double accel_y_seg2_min;
	double accel_y_seg3_max;
	double accel_y_seg3_min;

	double gyro_y_seg0_max;
	double gyro_y_seg0_min;
	double gyro_y_seg1_max;
	double gyro_y_seg1_min;
	double gyro_y_seg2_max;
	double gyro_y_seg2_min;
	double gyro_y_seg3_max;
	double gyro_y_seg3_min;

	double gyro_y_abs_integral;
	double period;
} GlobalFeature;

typedef struct
{
	double accel_x_seg0_mean;
	double accel_x_seg0_var;
	double accel_x_seg1_mean;
	double accel_x_seg1_var;
	double accel_x_seg2_mean;
	double accel_x_seg2_var;
	double accel_x_seg3_mean;
	double accel_x_seg3_var;
	double period;
} WalkFeature;


//Feature* extract_feature(double* data, double* time, int* S_i, int n_S, int feature_case);

void segmentation(int* pos, int start_pos, int end_pos);

GlobalFeature* extract_global_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S);

WalkFeature* extract_walk_feature(double* accel_x, double* time, int* S_i, int n_S);

/*
void turn_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_turn_feature_name);
void stair_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_stair_feature_name);
void jump_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_jump_feature_name);
void run_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_run_feature_name);
*/


#endif
