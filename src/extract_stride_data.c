/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
/* for fabsf() */
#include <math.h>

#define BUFF_SIZE 1024

/*
 * sets first <n> values in <*arr> to <val>
 */
void clear_buffer(int *arr, int val, int n) 
{
	int i;
	for (i = 0; i < n; i++) {
		arr[i] = val;
	}
}

/*
 * Caculates mean of first <n> samples in <*arr>
 */
/*float calculate_mean(float *arr, int n)
{
	float total;
	int i;

	total = 0.0f;
	for (i = 0; i < n; i++) {
		total += arr[i];
	}

	return total/((float) n);
}*/

int 
find_peaks_and_troughs(
		double* arr, 	// signal 
		int n_samples, 	// number of samples present in the signal
		float E, 	// threshold for peak detection
		// arrays that will store the indicies of the located
		// peaks and troughs
		int *P, int *T,
		// number of peaks (n_P) and number of troughs (n_T)
		// found in the data set *arr
		int *n_P, int *n_T
		)
{
	int a, b, i, d, _n_P, _n_T;

	i = -1; d = 0; a = 0; b = 0;
	_n_P = 0; _n_T = 0;

	clear_buffer(P, 0, n_samples);
	clear_buffer(T, 0, n_samples);

	while (i != n_samples) {
		i++;
		if (d == 0) {
			if (arr[a] >= (arr[i] + E)) {
				d = 2;
			} else if (arr[i] >= (arr[b] + E)) {
				d = 1;
			}
			if (arr[a] <= arr[i]) {
				a = i;
			} else if (arr[i] <= arr[b]) {
				b = i;
			}
		} else if (d == 1) {
			if (arr[a] <= arr[i]) {
				a = i;
			} else if (arr[a] >= (arr[i] + E)) {
				/*
				 * Peak has been detected.
				 * Add index at detected peak
				 * to array of peak indicies
				 * increment count of peak indicies
				 */
				P[_n_P] = a;
				_n_P++;
				b = i;
				d = 2;
			}
		} else if (d == 2) {
			if (arr[i] <= arr[b]) {
				b = i;
			} else if (arr[i] >= (arr[b] + E)) {
				/*
				 * Trough has been detected.
				 * Add index at detected trough
				 * to array of trough indicies
				 * increment count of trough indicies
				 */
				T[_n_T] = b;
				_n_T++;
				a = i;
				d = 1;
			}
		}
	}

	(*n_P) = _n_P;
	(*n_T) = _n_T;
	return 0;
}

int find_max_min(double* data, double* time, int* maxima, int* minima, int* stride, int stride_size)
{
	int i, j;
	for(i = 1; i< stride_size; i++){
		int local_max = stride[i-1];
		int local_min = stride[i-1];
	
		for(j = stride[i-1];j < stride[i]; j++){
			if(data[j] > data[local_max] && data[j] != data[stride[i-1]]){
				local_max = j;
			}
			if(data[j] < data[local_min])
				local_min = j;
			if(time[j] >= (time[stride[i-1]] + time[stride[i]])/2.0)
				break;
		}
		maxima[i-1] = local_max;
		minima[i-1] = local_min;
	}

	return 1;
}


int main(int argc, char **argv)
{
	/* Generic variables */
	int i, idx;
	int rv;
	
	/*input file name*/
	char* ifile_name; // data file
	
	/*output file names*/
	char* ofile_pt_name; // peak&trough file
	char* ofile_st_name; // stride detection file
	char* ofile_maxmin_name; // feature maxmin file(t, max, t, min)
	char *ofile_feature_name; // feature file(max min period)
	
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int N_SAMPLES;

	// Original Data Containers
	double* time;
	double* accel_x;
	double* accel_y;
	double* accel_z;
	double* gyro_x;
	double* gyro_y;
	double* gyro_z;

	float pk_threshold;
	float time_constraint;

	double t1, t2; // variable used to parse time_before and time_after
	double start_time; // variable used to computer sampling time
       	/* Variables for peak-trough detection */	
	int *P_i; 	// indicies of each peak found by peak detection
	int *T_i; 	// indicies of each trough found by trough detection
	int *S_i; 	// indicies of the start of each stride
	
	// Feature needed
	int *maxima_accel_y;
	int *minima_accel_y;
	int *maxima_gyro_y;
	int *minima_gyro_y;
	double* period;

	int n_P; 	// number of peaks
	int n_T; 	// number of troughs
	int n_S; 	// number of strides

	/*
	 * set this to 0 so that the code can function without
	 * having to actually performing stride detection
	 * from peaks and troughs
	 */
	n_S = 0; 
	
	/*
	 * Check if the user entered the correct command line arguments
	 * Usage: 
	 * ./extract_stride_data <ifile_name> <output_peaks> <output_strides> <output maxmin file>
	 * 				<output feature file> <threshold_value_float> <time constraint>
	 * 
	 */
	if (argc != 8) {
		fprintf(stderr, "USEAGE: ./extract_stride_data <input file> <peak trough detection output file> <stride detection output file> <maxmin output file> <feature output file> <threshold> <time constraints>");
		exit(EXIT_FAILURE);

	} else {
		ifile_name = argv[1];
		ofile_pt_name = argv[2];
		ofile_st_name = argv[3];
		ofile_maxmin_name = argv[4];
		ofile_feature_name = argv[5];
		pk_threshold = atof(argv[6]);
		time_constraint = atof(argv[7]);
	}

	printf("Arguments used:\n\t%s=%s\n\t%s=%s\n\t%s=%s\n\t%s=%f\t\n%s=%f\n",
			"ifile_name", ifile_name,
			"ofile_peak_trough_name", ofile_pt_name,
			"ofile_stride_name", ofile_st_name,
			"peak_threshold", pk_threshold,
			"time_contraint", time_constraint
	      );

	/* open the input file */
	printf("Attempting to read from file \'%s\'.\n", ifile_name);
	fp = fopen(ifile_name, "r");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to read from file \'%s\'.\n", 
				ifile_name
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

	/* 
	 * From selected thresholds, 
	 * find indicies of peaks
	 * find indicies of troughs
	 */
	P_i = (int *) malloc(sizeof(int) * N_SAMPLES);
	T_i = (int *) malloc(sizeof(int) * N_SAMPLES);
	

	rv = find_peaks_and_troughs(
			gyro_z, 
			N_SAMPLES, 
			pk_threshold, 
			P_i, T_i, 
			&n_P, &n_T);
	if (rv < 0) {
		fprintf(stderr, "find_peaks_and_troughs failed\n");
		exit(EXIT_FAILURE);
	}
	P_i = (int *) realloc(P_i, sizeof(int) * n_P);
	T_i = (int *) realloc(T_i, sizeof(int) * n_T);


	// Sort peaks to filter out inappropriate values
	printf("Attempting to sort.\n");
	S_i = (int *) malloc(sizeof(int) * (n_P));

	int idx_p;
	i = 0;
	while(i < n_P){
		idx_p = P_i[i];
		
		if(n_S==0){
			S_i[n_S] = P_i[i];
			i++;
			n_S++;
		}
		// if peak is far enough from previous peak
		if(time[idx_p] - time[S_i[n_S - 1]] >= time_constraint){
			S_i[n_S] = P_i[i];
			i++;
			n_S++;
		}
		// peak is too close to previous peak, skip
		else
			i++;
	}



	/* feature detection section*/
	// compute period
	period = (double*) malloc(sizeof(double)*(n_S - 1));
	for(i = 0; i < n_S-1; i++){
		period[i] = time[S_i[i+1]] - time[S_i[i]];
	}



	// extract maxima and minima
	maxima_accel_y = (int *) malloc(sizeof(int) * (n_S));
	minima_accel_y = (int *) malloc(sizeof(int) * (n_S));
	maxima_gyro_y = (int *) malloc(sizeof(int) * (n_S));
	minima_gyro_y = (int*) malloc(sizeof(int) * (n_S));

	find_max_min(gyro_y, time, maxima_gyro_y, minima_gyro_y, S_i, n_S);
	find_max_min(accel_x, time, maxima_accel_y, minima_accel_y, S_i, n_S);

	/* open the output file to write the peak and trough data */
	printf("Attempting to write to file \'%s\'.\n", ofile_pt_name);
	fp = fopen(ofile_pt_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_pt_name
		       );
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "P_t,P_x,T_t,T_p\n");
	for (i = 0; i < n_P || i < n_T; i++) {
		/* Only peak data if there is peak data to write */
		if (i < n_P) {
			idx = P_i[i];
			fprintf(fp, "%20.10lf,%lf,",
					time[idx],
				gyro_z[idx]);
		} else {
			fprintf(fp, ",,,");
		}
		/* Only trough data if there is trough data to write */
		if (i < n_T) {
			idx = T_i[i];
			fprintf(fp, "%20.10lf,%lf\n",
					time[idx],
					gyro_z[idx]);
		} else {
			fprintf(fp, ",,\n");
		}
	}
	fclose(fp);

	/* open the output file to write the stride data */
	printf("Attempting to write to file \'%s\'.\n", ofile_st_name);
	fp = fopen(ofile_st_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_st_name
		       );

		exit(EXIT_FAILURE);
	}

	fprintf(fp, "S_t,S_x\n");
	for (i = 0; i < n_S; i++) {
		idx = S_i[i];
		fprintf(fp, "%20.10lf,%lf\n",
				time[idx],
				gyro_z[idx]);
	}
	fclose(fp);
	
	printf("Attempting to write to file \'%s\'.\n", ofile_maxmin_name);
	fp = fopen(ofile_maxmin_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_maxmin_name
		       );

		exit(EXIT_FAILURE);
	}
	fprintf(fp, "T_max_accel_y,Max_accel_y,T_min_accel_y,Min_accel_y,T_max_gyro_y,Max_gyro_y,T_min_gyro_y,Min_gyro_y\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%20.10lf,%lf,%20.10lf,%lf,%20.10lf,%lf,%20.10lf,%lf\n", 
			time[maxima_accel_y[i]], accel_y[maxima_accel_y[i]], 
			time[minima_accel_y[i]], accel_y[minima_accel_y[i]],
			time[maxima_gyro_y[i]], gyro_y[maxima_gyro_y[i]], 
			time[minima_gyro_y[i]], gyro_y[minima_gyro_y[i]]);
	}
	fclose(fp);

	printf("Attempting to write to file \'%s\'.\n", ofile_feature_name);
	fp = fopen(ofile_feature_name, "w");
	if (fp == NULL) {
		fprintf(stderr, 
				"Failed to write to file \'%s\'.\n", 
				ofile_st_name
		       );

		exit(EXIT_FAILURE);
	}
	fprintf(fp, "Max_accel_y,Min_accel_y,Max_gyro_y,Min_gyro_y,Period\n");
	for(i = 0; i < n_S-1; i++){
		fprintf(fp, "%lf,%lf,%lf,%lf,%20.10lf\n", accel_x[maxima_accel_y[i]], accel_x[minima_accel_y[i]], gyro_y[maxima_gyro_y[i]], gyro_y[minima_gyro_y[i]], period[i]);
	}
	fclose(fp);


	printf("extract_stride_data completed successfuly. Exiting.\n");

	return 0;
}
