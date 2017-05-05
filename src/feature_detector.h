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

typedef struct{
	double gyro_y_seg0_max;
	double gyro_y_seg0_min;
	double gyro_y_seg1_max;
	double gyro_y_seg1_min;
	double gyro_y_seg2_max;
	double gyro_y_seg2_min;
	double gyro_y_seg3_max;
	double gyro_y_seg3_min;

	double gyro_y_integral;
} TurnFeature;

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




void segmentation(int* pos, int start_pos, int end_pos);

void extract_global_feature(GlobalFeature* feature, int* pos, double* accel_y, double* gyro_y, double* time);

void extract_turn_feature(TurnFeature* feature, int *pos, double* gyro_y, double* time);

void extract_walk_feature(WalkFeature* feature, int *pos, double* accel_x, double* time);

/*
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
