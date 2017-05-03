#include "feature_detector.h"
// Divide the stride into 4 segmentations
// Extract max min on each segmentation
GlobalFeature* extract_global_feature(double *accel_y, double *gyro_y, double *time, int *S_i, int n_S, GlobalFeatureScale* scale)
{
	int i;
	GlobalFeature *features = (GlobalFeature*) malloc(sizeof(GlobalFeature) * n_S-1);
	scale = (GlobalFeatureScale*) malloc(sizeof(GlobalFeatureScale));
	scale->gyro_y_abs_integral_scale = 0;
	scale->period_scale = 0;
	
	for(i = 0; i< n_S-1; i++){

		// Segmentation
		float step = (float)(S_i[i+1] - S_i[i]);
		int pos0 = S_i[i];
		int pos1 = S_i[i] + (int)(step / 4.0);
		int pos2 = S_i[i] + (int)(step / 2.0);
		int pos3 = S_i[i] + (int)(3.0 * step / 4.0);
		int pos4 = S_i[i+1];

		
		max(accel_y, pos0, pos1, &features[i].accel_y_seg0_max);
		min(accel_y, pos0, pos1, &features[i].accel_y_seg0_min);
		max(accel_y, pos1, pos2, &features[i].accel_y_seg1_max);
		min(accel_y, pos1, pos2, &features[i].accel_y_seg1_min);
		max(accel_y, pos2, pos3, &features[i].accel_y_seg2_max);
		min(accel_y, pos2, pos3, &features[i].accel_y_seg2_min);
		max(accel_y, pos3, pos4, &features[i].accel_y_seg3_max);
		min(accel_y, pos3, pos4, &features[i].accel_y_seg3_min);

		max(gyro_y, pos0, pos1, &features[i].gyro_y_seg0_max);
		min(gyro_y, pos0, pos1, &features[i].gyro_y_seg0_min);
		max(gyro_y, pos1, pos2, &features[i].gyro_y_seg1_max);
		min(gyro_y, pos1, pos2, &features[i].gyro_y_seg1_min);
		max(gyro_y, pos2, pos3, &features[i].gyro_y_seg2_max);
		min(gyro_y, pos2, pos3, &features[i].gyro_y_seg2_min);
		max(gyro_y, pos3, pos4, &features[i].gyro_y_seg3_max);
		min(gyro_y, pos3, pos4, &features[i].gyro_y_seg3_min);
		
		integral(gyro_y, time, pos0, pos4, &features[i].gyro_y_abs_integral);
		features[i].gyro_y_abs_integral = fabs(features[i].gyro_y_abs_integral);
		if(features[i].gyro_y_abs_integral > scale->gyro_y_abs_integral_scale)
			scale->gyro_y_abs_integral_scale = features[i].gyro_y_abs_integral;

		features[i].period = time[S_i[i+1]] - time[S_i[i]];
		if(features[i].period > scale->period_scale)
			scale->period_scale = features[i].period;
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

