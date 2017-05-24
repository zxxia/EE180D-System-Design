#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H
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
	double accel_x_seg0_max, accel_x_seg0_min, accel_x_seg0_rms;
	double accel_x_seg1_max, accel_x_seg1_min, accel_x_seg1_rms;
	double accel_x_seg2_max, accel_x_seg2_min, accel_x_seg2_rms;
	double accel_x_seg3_max, accel_x_seg3_min, accel_x_seg3_rms;
	double period;
} WalkFeature;


typedef struct
{
	double accel_x_seg0_max, accel_x_seg0_min;
	double accel_x_seg1_max, accel_x_seg1_min;
	double accel_x_seg2_max, accel_x_seg2_min;
	double accel_x_seg3_max, accel_x_seg3_min;

	double accel_y_seg0_max, accel_y_seg0_min;
	double accel_y_seg1_max, accel_y_seg1_min;
	double accel_y_seg2_max, accel_y_seg2_min;
	double accel_y_seg3_max, accel_y_seg3_min;
} StairFeature;



void segmentation(int* pos, int start_pos, int end_pos);

void extract_global_feature(GlobalFeature* feature, int* pos, double* accel_y, double* gyro_y, double* t);

void extract_turn_feature(TurnFeature* feature, int *pos, double* gyro_y, double* t);

void extract_walk_feature(WalkFeature* feature, int *pos, double* accel_x, double* t);

void extract_stair_feature(StairFeature* feature, int* pos, double* accel_x, double* accel_y, double* t);


/*void jump_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_jump_feature_name);
void run_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, 
	char* ofile_run_feature_name);
*/


#endif
