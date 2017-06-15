#include "feature_detector.h"


// Divide a stride into 4 segmentations
void segmentation(int* pos, int start_pos, int end_pos)
{
	float step = (float)(end_pos - start_pos);
	pos[0] = start_pos;
	pos[1] = start_pos + (int)(step / 4.0);
	pos[2] = start_pos + (int)(step / 2.0);
	pos[3] = start_pos + (int)(3.0 * step / 4.0);
	pos[4] = end_pos;
}

void extract_global_feature(GlobalFeature* feature, int *pos, double *accel_y, double *gyro_y, double *t, double *gyro_z)
{
	// Extract features
	max(accel_y, pos[0], pos[1], &(feature->accel_y_seg0_max));
	min(accel_y, pos[0], pos[1], &(feature->accel_y_seg0_min));
	max(accel_y, pos[1], pos[2], &(feature->accel_y_seg1_max));
	min(accel_y, pos[1], pos[2], &(feature->accel_y_seg1_min));
	max(accel_y, pos[2], pos[3], &(feature->accel_y_seg2_max));
	min(accel_y, pos[2], pos[3], &(feature->accel_y_seg2_min));
	max(accel_y, pos[3], pos[4], &(feature->accel_y_seg3_max));
	min(accel_y, pos[3], pos[4], &(feature->accel_y_seg3_min));
	
	integral(gyro_y, t, pos[0], pos[4], &(feature->gyro_y_abs_integral));
	feature->gyro_y_abs_integral = fabs(feature->gyro_y_abs_integral);

	feature->period = t[pos[4]] - t[pos[0]];

	skewness(t, pos[0], pos[4], &(feature->gyro_z_skewness));

	double* positive_time = (double*)malloc(sizeof(double)*(pos[4] - pos[0]));
	double* positive_val = (double*)malloc(sizeof(double)*(pos[4] - pos[0]));
	
	int n = 0;
	int j;
	for(j = pos[0]; j < pos[4]; j++){
		if(gyro_z[j] > 0){
			positive_time[n] = t[j];
			positive_val[n] = gyro_z[j];
			n++;
		}
	}

	skewness(positive_time, 0, n, &(feature->gyro_z_skewness_positive));


	max(gyro_z, pos[0], pos[4], &(feature->max));
	min(gyro_z, pos[0], pos[4], &(feature->min));

	feature->gyro_z_max_min_ratio = feature->max - feature->min;
}

void extract_turn_feature(TurnFeature* feature, int* pos, double *gyro_y, double *t)
{
	// Extract features
	max(gyro_y, pos[0], pos[1], &(feature->gyro_y_seg0_max));
	min(gyro_y, pos[0], pos[1], &(feature->gyro_y_seg0_min));
	max(gyro_y, pos[1], pos[2], &(feature->gyro_y_seg1_max));
	min(gyro_y, pos[1], pos[2], &(feature->gyro_y_seg1_min));
	max(gyro_y, pos[2], pos[3], &(feature->gyro_y_seg2_max));
	min(gyro_y, pos[2], pos[3], &(feature->gyro_y_seg2_min));
	max(gyro_y, pos[3], pos[4], &(feature->gyro_y_seg3_max));
	min(gyro_y, pos[3], pos[4], &(feature->gyro_y_seg3_min));

	integral(gyro_y, t, pos[0], pos[4], &(feature->gyro_y_integral));
}

void extract_walk_feature(WalkFeature *feature, int *pos, double *accel_x, double *t)
{
	// Extract features
	max(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_max));
	min(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_min));
	rms(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_rms));
	
	max(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_max));
	min(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_min));
	rms(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_rms));
	
	max(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_max));
	min(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_min));
	rms(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_rms));
	
	max(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_max));
	min(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_min));
	rms(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_rms));
	
	feature->period = t[pos[4]] - t[pos[0]];
}


void extract_stair_feature(StairFeature *feature, int *pos, double *accel_x, double *accel_y, double *t)
{
	// Extract features
	max(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_max));
	min(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_min));
	
	max(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_max));
	min(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_min));
	
	max(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_max));
	min(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_min));
	
	max(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_max));
	min(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_min));
	

	max(accel_y, pos[0], pos[1], &(feature->accel_y_seg0_max));
	min(accel_y, pos[0], pos[1], &(feature->accel_y_seg0_min));
	
	max(accel_y, pos[1], pos[2], &(feature->accel_y_seg1_max));
	min(accel_y, pos[1], pos[2], &(feature->accel_y_seg1_min));
	
	max(accel_y, pos[2], pos[3], &(feature->accel_y_seg2_max));
	min(accel_y, pos[2], pos[3], &(feature->accel_y_seg2_min));

	max(accel_y, pos[3], pos[4], &(feature->accel_y_seg3_max));
	min(accel_y, pos[3], pos[4], &(feature->accel_y_seg3_min));
}


void extract_run_feature(RunFeature *feature, int *pos, double *accel_x, double *t)
{
	// Extract features
	max(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_max));
	min(accel_x, pos[0], pos[1], &(feature->accel_x_seg0_min));
	
	max(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_max));
	min(accel_x, pos[1], pos[2], &(feature->accel_x_seg1_min));
	
	max(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_max));
	min(accel_x, pos[2], pos[3], &(feature->accel_x_seg2_min));
	
	max(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_max));
	min(accel_x, pos[3], pos[4], &(feature->accel_x_seg3_min));
	

	integral(accel_x, t, pos[0], pos[4], &(feature->accel_x_abs_integral));
	feature->accel_x_abs_integral = fabs(feature->accel_x_abs_integral);
	feature->period = t[pos[4]] - t[pos[0]];
}