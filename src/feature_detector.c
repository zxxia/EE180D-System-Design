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

void extract_global_feature(GlobalFeature* feature, int *pos, double *accel_y, double *gyro_y, double *time)
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

	max(gyro_y, pos[0], pos[1], &(feature->gyro_y_seg0_max));
	min(gyro_y, pos[0], pos[1], &(feature->gyro_y_seg0_min));
	max(gyro_y, pos[1], pos[2], &(feature->gyro_y_seg1_max));
	min(gyro_y, pos[1], pos[2], &(feature->gyro_y_seg1_min));
	max(gyro_y, pos[2], pos[3], &(feature->gyro_y_seg2_max));
	min(gyro_y, pos[2], pos[3], &(feature->gyro_y_seg2_min));
	max(gyro_y, pos[3], pos[4], &(feature->gyro_y_seg3_max));
	min(gyro_y, pos[3], pos[4], &(feature->gyro_y_seg3_min));
		
	integral(gyro_y, time, pos[0], pos[4], &(feature->gyro_y_abs_integral));
	feature->gyro_y_abs_integral = fabs(feature->gyro_y_abs_integral);

	feature->period = time[pos[4]] - time[pos[0]];
}

void extract_turn_feature(TurnFeature* feature, int* pos, double *gyro_y, double *time)
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

	integral(gyro_y, time, pos[0], pos[4], &(feature->gyro_y_integral));
}

void extract_walk_feature(WalkFeature *feature, int *pos, double *accel_x, double *time)
{
	// Extract features
	mean(accel_x, pos[0], pos[1], &feature->accel_x_seg0_mean);
	variance(accel_x, pos[0], pos[1], &feature->accel_x_seg0_var);
	mean(accel_x, pos[1], pos[2], &feature->accel_x_seg1_mean);
	variance(accel_x, pos[1], pos[2], &feature->accel_x_seg1_var);
	mean(accel_x, pos[2], pos[3], &feature->accel_x_seg2_mean);
	variance(accel_x, pos[2], pos[3], &feature->accel_x_seg2_var);
	mean(accel_x, pos[3], pos[4], &feature->accel_x_seg3_mean);
	variance(accel_x, pos[3], pos[4], &feature->accel_x_seg3_var);

	feature->period = time[pos[4]] - time[pos[0]];
}