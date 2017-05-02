#include "feature_detector.h"
// Divide the stride into 4 segmentations
// Extract max min on each segmentation
Feature* extract_feature(double* data, double* time, int* S_i, int n_S)
{
	int i;
	Feature *features = (Feature*) malloc(sizeof(Feature) * n_S-1);
	
	for(i = 0; i< n_S-1; i++){

		// Segmentation
		float step = (float)(S_i[i+1] - S_i[i]);
		int pos0 = S_i[i];
		int pos1 = S_i[i] + (int)(step / 4.0);
		int pos2 = S_i[i] + (int)(step / 2.0);
		int pos3 = S_i[i] + (int)(3.0 * step / 4.0);
		int pos4 = S_i[i+1];

		
		max(data, pos0, pos1, &features[i].seg0.max);
		min(data, pos0, pos1, &features[i].seg0.min);
		

		//Add seg max/min + period to array
		max(data, pos1, pos2, &features[i].seg1.max);
		min(data, pos1, pos2, &features[i].seg1.min);
		

		//Add seg max/min + period to array
		max(data, pos2, pos3, &features[i].seg2.max);
		min(data, pos2, pos3, &features[i].seg2.min);
		
		//Add seg max/min + period to array
		max(data, pos3, pos4, &features[i].seg3.max);
		min(data, pos3, pos4, &features[i].seg3.min);
		

		//Add seg max/min + period to array
		//mean(data, pos0, pos4, &features[i].abs_mean);
		
		integral(data, time, pos0, pos4, &features[i].abs_integral);
		features[i].abs_integral = fabs(features[i].abs_integral);
		//fprintf(stdout, "%lf\n", features[i].abs_mean);
		/*
		find_variance(data, pos0, pos1, &features[i].seg0_var);
		find_variance(data, pos1, pos2, &features[i].seg1_var);
		find_variance(data, pos2, pos3, &features[i].seg2_var);
		find_variance(data, pos3, pos4, &features[i].seg3_var);
		*/
	}

	return features;
}




void global_feature(double* accel_y, double* gyro_y,
	double* time, int* S_i, int n_S, char* ofile_maxmin_name, char* ofile_feature_name)
{
	/* Generic variables */
	int i;
	FILE *fp;

	// Feature needed
	Feature* features_accel_y;
	Feature* features_gyro_y;
	double* period;


	/* feature detection section*/
	// compute period
	period = (double*) malloc(sizeof(double)*(n_S - 1));
	for(i = 0; i < n_S-1; i++){
		period[i] = time[S_i[i+1]] - time[S_i[i]];
	}

	//2 indices divided by 4 
	features_gyro_y = extract_feature(gyro_y, time, S_i, n_S);
	features_accel_y = extract_feature(accel_y, time, S_i, n_S);

	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
	fp = fopen(ofile_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_feature_name
		       );
		exit(EXIT_FAILURE); 
	}

	fprintf(fp, "Seg0_Max,Seg0_Min,Seg1_Max,Seg1_Min,Seg2_Max,Seg2_Min,Seg3_Max,Seg3_Min,Period\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%20.10lf\n",
		 	features_accel_y[i].seg0.max, features_accel_y[i].seg0.min,
			features_accel_y[i].seg1.max, features_accel_y[i].seg1.min,
			features_accel_y[i].seg2.max, features_accel_y[i].seg2.min,
			features_accel_y[i].seg3.max, features_accel_y[i].seg3.min,
			features_gyro_y[i].seg0.max, features_gyro_y[i].seg0.min,
			features_gyro_y[i].seg1.max, features_gyro_y[i].seg1.min,
			features_gyro_y[i].seg2.max, features_gyro_y[i].seg2.min,
			features_gyro_y[i].seg3.max, features_gyro_y[i].seg3.min,
			features_gyro_y[i].abs_integral, period[i]);
	}
	fclose(fp);

	//return 0;
}
