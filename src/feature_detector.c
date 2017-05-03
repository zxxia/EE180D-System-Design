#include "feature_detector.h"
// Divide the stride into 4 segmentations
// Extract max min on each segmentation

void segmentation(int* pos, int start_pos, int end_pos)
{
	float step = (float)(end_pos - start_pos);
	pos[0] = start_pos;
	pos[1] = start_pos + (int)(step / 4.0);
	pos[2] = start_pos + (int)(step / 2.0);
	pos[3] = start_pos + (int)(3.0 * step / 4.0);
	pos[4] = end_pos;
}

GlobalFeature* extract_global_feature(double *accel_y, double *gyro_y, double *time, int *S_i, int n_S)
{
	int i;
	GlobalFeature *features = (GlobalFeature*) malloc(sizeof(GlobalFeature) * n_S-1);

	int pos[5];
	for(i = 0; i< n_S-1; i++){

		// stride segmentation
		segmentation(pos, S_i[i], S_i[i+1]);

		// Extract features
		max(accel_y, pos[0], pos[1], &features[i].accel_y_seg0_max);
		min(accel_y, pos[0], pos[1], &features[i].accel_y_seg0_min);
		max(accel_y, pos[1], pos[2], &features[i].accel_y_seg1_max);
		min(accel_y, pos[1], pos[2], &features[i].accel_y_seg1_min);
		max(accel_y, pos[2], pos[3], &features[i].accel_y_seg2_max);
		min(accel_y, pos[2], pos[3], &features[i].accel_y_seg2_min);
		max(accel_y, pos[3], pos[4], &features[i].accel_y_seg3_max);
		min(accel_y, pos[3], pos[4], &features[i].accel_y_seg3_min);

		max(gyro_y, pos[0], pos[1], &features[i].gyro_y_seg0_max);
		min(gyro_y, pos[0], pos[1], &features[i].gyro_y_seg0_min);
		max(gyro_y, pos[1], pos[2], &features[i].gyro_y_seg1_max);
		min(gyro_y, pos[1], pos[2], &features[i].gyro_y_seg1_min);
		max(gyro_y, pos[2], pos[3], &features[i].gyro_y_seg2_max);
		min(gyro_y, pos[2], pos[3], &features[i].gyro_y_seg2_min);
		max(gyro_y, pos[3], pos[4], &features[i].gyro_y_seg3_max);
		min(gyro_y, pos[3], pos[4], &features[i].gyro_y_seg3_min);
		
		integral(gyro_y, time, pos[0], pos[4], &features[i].gyro_y_abs_integral);
		features[i].gyro_y_abs_integral = fabs(features[i].gyro_y_abs_integral);

		features[i].period = time[S_i[i+1]] - time[S_i[i]];
	}

	return features;
}


WalkFeature* extract_walk_feature(double *accel_x, double *time, int *S_i, int n_S)
{
	int i;
	int pos[5];
	WalkFeature *features = (WalkFeature*) malloc(sizeof(WalkFeature) * n_S-1);
	
	for(i = 0; i< n_S-1; i++){

		// Segmentation
		segmentation(pos, S_i[i], S_i[i+1]);
		
		mean(accel_x, pos[0], pos[1], &features[i].accel_x_seg0_mean);
		variance(accel_x, pos[0], pos[1], &features[i].accel_x_seg0_var);
		mean(accel_x, pos[1], pos[2], &features[i].accel_x_seg1_mean);
		variance(accel_x, pos[1], pos[2], &features[i].accel_x_seg1_var);
		mean(accel_x, pos[2], pos[3], &features[i].accel_x_seg2_mean);
		variance(accel_x, pos[2], pos[3], &features[i].accel_x_seg2_var);
		mean(accel_x, pos[3], pos[4], &features[i].accel_x_seg3_mean);
		variance(accel_x, pos[3], pos[4], &features[i].accel_x_seg3_var);

		features[i].period = time[S_i[i+1]] - time[S_i[i]];
	}
	return features;
}




// void global_feature(double* accel_y, double* gyro_y,
// 	double* time, int* S_i, int n_S, const char* ofile_feature_name)
// {
// 	/* Generic variables */
// 	int i;
// 	FILE *fp;

// 	// Feature needed
// 	Global_feature* feature;
// 	/* feature detection section*/

// 	//2 indices divided by 4 
// 	feature.features_gyro_y = extract_feature(gyro_y, time, S_i, n_S, GLOBAL);
// 	feature.features_accel_y = extract_feature(accel_y, time, S_i, n_S, GLOBAL);
// 	feature.period = (double*) malloc(sizeof(double)*(n_S - 1));
// 	for(i = 0; i < n_S-1; i++){
// 		feature.period[i] = time[S_i[i+1]] - time[S_i[i]];
// 	}


// 	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
// 	fp = fopen(ofile_feature_name, "w");
// 	if (fp == NULL) {
// 		fprintf(stderr, "Failed to write to file \'%s\'.\n", 
// 				ofile_feature_name);
// 		exit(EXIT_FAILURE); 
// 	}

// 	fprintf(fp, "Seg0_Max,Seg0_Min,Seg1_Max,Seg1_Min,Seg2_Max,Seg2_Min,Seg3_Max,Seg3_Min,Period\n");
// 	for(i = 0; i < n_S-1; i++){
// 		fprintf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%20.10lf\n",
// 		 	features_accel_y[i].seg0.max/6.0, features_accel_y[i].seg0.min/6.0,
// 			features_accel_y[i].seg1.max/6.0, features_accel_y[i].seg1.min/6.0,
// 			features_accel_y[i].seg2.max/6.0, features_accel_y[i].seg2.min/6.0,
// 			features_accel_y[i].seg3.max/6.0, features_accel_y[i].seg3.min/6.0,
// 			features_gyro_y[i].seg0.max/500.0, features_gyro_y[i].seg0.min/500.0,
// 			features_gyro_y[i].seg1.max/500.0, features_gyro_y[i].seg1.min/500.0,
// 			features_gyro_y[i].seg2.max/500.0, features_gyro_y[i].seg2.min/500.0,
// 			features_gyro_y[i].seg3.max/500.0, features_gyro_y[i].seg3.min/500.0,
// 			features_gyro_y[i].abs_integral, features_gyro_y[i].period);
// 	}
// 	fclose(fp);

// }

