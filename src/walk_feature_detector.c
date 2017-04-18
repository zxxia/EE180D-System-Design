/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
/* for fabsf() */
#include <math.h>

struct Feature{
	// segmentaion 0
	double seg0_max;
	double seg0_min;
	double seg0_mean;
	double seg0_var;
	double seg0_rms;
	double seg0_mad;

	// segmentation 1
	double seg1_max;
	double seg1_min;
	double seg1_mean;
	double seg1_var;
	double seg1_rms;
	double seg1_mad;

	// segmentation 2
	double seg2_max;
	double seg2_min;
	double seg2_mean;
	double seg2_var;
	double seg2_rms;
	double seg2_mad;

	// segmentation 3
	double seg3_max;
	double seg3_min;
	double seg3_mean;
	double seg3_var;
	double seg3_rms;
	double seg3_mad;
};

//Max-min feature
void find_max_min(double* data, int start_pos, int end_pos, double* max, double* min)
{
	int i;
	*max = data[start_pos];
	*min = data[start_pos];
	for(i = start_pos; i < end_pos; i++){
		if(data[i] > *max)
			*max = data[i];
		if(data[i] < *min)
			*min = data[i];
	}
}

//Mean
void find_mean(double* data, int start_pos, int end_pos, double* mean)
{
	int i;
	int totalPos = end_pos - start_pos;
	double sum = 0;
	for(i = start_pos; i < end_pos; i++) {
		sum = sum + data[i];
	}
	*mean = sum / (double)totalPos;
}

//Variance feature
void find_variance(double* data, int start_pos, int end_pos, double* var)
{
	int i;
	int totalPos = end_pos - start_pos;
	double mean, sum = 0;
	find_mean(data, start_pos, end_pos, &mean);
	for(i = start_pos; i < end_pos; i++) {
		sum = sum + pow((data[i] - mean), 2);
	}
	*var = sum / (double)totalPos;
}

//STD
void  find_std(double* data, int start_pos, int end_pos, double* std)
{
	double var;
	find_variance(data, start_pos, end_pos, &var);
	*std = sqrt(var);
}

//RMS
void find_rms(double* data, int start_pos, int end_pos, double* rms)
{
	int i;
	int totalPos = end_pos - start_pos;
	double sum = 0;
	for(i = start_pos; i < end_pos; i++) {
		sum = sum + pow(data[i], 2);
	}
	*rms = pow((sum / (double)totalPos), 0.5);
}

//MAD
void find_mad(double* data, int start_pos, int end_pos, double* mad)
{
	int i;
	int totalPos = end_pos - start_pos;
	double mean, sum = 0;
	find_mean(data, start_pos, end_pos, &mean);
	for (i = start_pos; i < end_pos; i++) {
		sum = sum + fabs(data[i] - mean);
	}
	*mad = sum / (double)totalPos;
}

//Max-min ratio

//Skewness?????Not a good one????
//Kurtosis?????Consider for later
//Correlation??????????

// Divide the stride into 4 segmentations
// Extract max min on each segmentation
struct Feature* extract_feature(double* data, double* time, int* S_i, int n_S)
{
	int i;
	struct Feature *features = (struct Feature*) malloc(sizeof(struct Feature) * n_S-1);
	
	for(i = 0; i< n_S-1; i++){

		// Segmentation
		float step = (float)(S_i[i+1] - S_i[i]);
		int pos0 = S_i[i];
		int pos1 = S_i[i] + (int)(step / 4.0);
		int pos2 = S_i[i] + (int)(step / 2.0);
		int pos3 = S_i[i] + (int)(3.0 * step / 4.0);
		int pos4 = S_i[i+1];

		/*
		find_max_min(data, pos0, pos1, &features[i].seg0_max, &features[i].seg0_min);
		//Add seg max/min + period to array
		find_max_min(data, pos1, pos2, &features[i].seg1_max, &features[i].seg1_min);
		//Add seg max/min + period to array
		find_max_min(data, pos2, pos3, &features[i].seg2_max, &features[i].seg2_min);
		//Add seg max/min + period to array
		find_max_min(data, pos3, pos4, &features[i].seg3_max, &features[i].seg3_min);
		//Add seg max/min + period to array
		*/
		
		
		find_variance(data, pos0, pos1, &features[i].seg0_var);
		find_variance(data, pos1, pos2, &features[i].seg1_var);
		find_variance(data, pos2, pos3, &features[i].seg2_var);
		find_variance(data, pos3, pos4, &features[i].seg3_var);
		
	}

	return features;
}




int main(int argc, char **argv)
{
	/* Generic variables */
	int i;
	int rv;
	
	/*input file name*/
	char* ifile_data_name; // data file
	char* ifile_st_name; // stride detection file
	
	/*output file names*/
	char* ofile_maxmin_name; // feature maxmin file(t, max, t, min)
	char* ofile_feature_name; // feature file(max min period)

	
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int N_SAMPLES; // number of samples
	int n_S; 	// number of strides


	double t1, t2; // variable used to parse time_before and time_after
	double start_time; // variable used to computer sampling time

	// Original Data Containers
	double* time;
	double* accel_x;
	double* accel_y;
	double* accel_z;
	double* gyro_x;
	double* gyro_y;
	double* gyro_z;

	int *S_i; 	// indicies of the start of each stride
	double *S_t; // time of the start of each stride
	double *S_val; // value of gyro_z of the start of each stride

	// Feature needed
	struct Feature* features0;
	struct Feature* features1;
	/*int *maxima_accel_y;
	int *minima_accel_y;
	int *maxima_gyro_y;
	int *minima_gyro_y;*/
	double* period;


	/*
	 * Check if the user entered the correct command line arguments
	 */
	if (argc != 5) {
		fprintf(stderr, "USEAGE: ./feature_detector <input data file> <input stride file> <output maxmin file> <output feature file>\n");
		exit(EXIT_FAILURE);

	} else {
		ifile_data_name = argv[1];
		ifile_st_name = argv[2];
		ofile_maxmin_name = argv[3];
		ofile_feature_name = argv[4];
	}

	printf("Arguments used:\n\t%s=%s\n\t%s=%s\n\t%s=%s\n\t%s=%s\t\n",
		"ifile_data_name", ifile_data_name,
		"ifile_stride_name", ifile_st_name,
		"ofile_maxmin_name", ofile_maxmin_name,
		"ofile_feature_name", ofile_feature_name);

	/* open the input data file */
	printf("Attempting to read from file \'%s\'.\n", ifile_data_name);
	fp = fopen(ifile_data_name, "r");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to read from file \'%s\'.\n", 
				ifile_data_name
		       );
		exit(EXIT_FAILURE);
	}

	/* count the number of lines in the file */
	read = getline(&line, &len, fp); //discard header of file
	N_SAMPLES = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		N_SAMPLES++;
	}

	/* go back to the start of the file so that the data can be read */
	rewind(fp);
	read = getline(&line, &len, fp); //discard header of file

	/* start reading the data from the file into the data structures */
	i = 0;

	// allocate memory
	time = (double*) malloc(sizeof(double) * N_SAMPLES);
	accel_x = (double*) malloc(sizeof(double) * N_SAMPLES);
	accel_y = (double*) malloc(sizeof(double) * N_SAMPLES);
	accel_z = (double*) malloc(sizeof(double) * N_SAMPLES);
	gyro_x = (double*) malloc(sizeof(double) * N_SAMPLES);
	gyro_y = (double*) malloc(sizeof(double) * N_SAMPLES);
	gyro_z = (double*) malloc(sizeof(double) * N_SAMPLES);

	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &t1, &t2, &accel_x[i], &accel_y[i], &accel_z[i], &gyro_x[i], &gyro_y[i], &gyro_z[i]);
		if (rv != 8) {
			fprintf(stderr,
					"%s %d \'%s\'. %s.\n",
					"Failed to read line",
					i,
					line,
					"Exiting"
			       );
			exit(EXIT_FAILURE);
		}
		if(i == 0)
			start_time = t1;

		//fprintf(stdout, "%20.10lf, %20.10lf\n", t1, t2);
		time[i] = (t1 + t2)/2.0 - start_time;
		i++;
	}
	fclose(fp);



	/* open the input stride file */
	printf("Attempting to read from file \'%s\'.\n", ifile_st_name);
	fp = fopen(ifile_st_name, "r");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to read from file \'%s\'.\n", 
				ifile_st_name
		       );
		exit(EXIT_FAILURE);
	}


	/* count the number of lines in the file */
	read = getline(&line, &len, fp); //discard header of file
	n_S = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		n_S++;
	}

	/* go back to the start of the file so that the data can be read */
	rewind(fp);
	read = getline(&line, &len, fp); //discard header of file

	/* start reading the data from the file into the data structures */
	i = 0;

	// allocate memory
	S_i = (int*)malloc(sizeof(int) * n_S);
	S_t = (double*)malloc(sizeof(double) * n_S);
	S_val =(double*)malloc(sizeof(double) * n_S);

	// read stride information
	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%d,%lf,%lf\n", &S_i[i], &S_t[i], &S_val[i]);
		if (rv != 3) {
			fprintf(stderr,
					"%s %d \'%s\'. %s.\n",
					"Failed to read line",
					i,
					line,
					"Exiting"
			       );
			exit(EXIT_FAILURE);
		}
		i++;
	}
	fclose(fp);



	/* feature detection section*/
	// compute period
	period = (double*) malloc(sizeof(double)*(n_S - 1));
	for(i = 0; i < n_S-1; i++){
		period[i] = time[S_i[i+1]] - time[S_i[i]];
	}

	//2 indices divided by 4 
	//features1 = extract_feature(gyro_y, time, S_i, n_S);
	features0 = extract_feature(accel_x, time, S_i, n_S);

	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
	fp = fopen(ofile_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_feature_name
		       );
		exit(EXIT_FAILURE); 
	}

	//TODO: Further change on header line
	fprintf(fp, "Seg0_Max,Seg0_Min,Seg1_Max,Seg1_Min,Seg2_Max,Seg2_Min,Seg3_Max,Seg3_Min,Period\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%lf,%lf,%lf,%lf,%20.10lf\n",
		 	features0[i].seg0_var, features0[i].seg1_var, 
			features0[i].seg2_var, features0[i].seg3_var,
			period[i]);
	}



	fclose(fp);




	return 0;
}