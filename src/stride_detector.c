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
double calculate_mean(double *arr, int n)
{
	double total;
	int i;

	total = 0.0f;
	for (i = 0; i < n; i++) {
		total += arr[i];
	}

	return total/((double) n);
}

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

	double t1, t2; // variable used to parse time_before and time_after
	double start_time; // variable used to computer sampling time
       	/* Variables for peak-trough detection */	
	int *P_i; 	// indicies of each peak found by peak detection
	int *T_i; 	// indicies of each trough found by trough detection
	int *S_i; 	// indicies of the start of each stride

	int n_P; 	// number of peaks
	int n_T; 	// number of troughs
	int n_S; 	// number of strides

	double* temp;
	double mean;

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
	if (argc != 5) {
		fprintf(stderr, "USEAGE: ./stride_detector <input data file> <peak trough detection output file> <stride detection output file> <threshold>\n");
		exit(EXIT_FAILURE);

	} else {
		ifile_name = argv[1];
		ofile_pt_name = argv[2];
		ofile_st_name = argv[3];
		pk_threshold = atof(argv[4]);
	}

	printf("Arguments used:\n\t%s=%s\n\t%s=%s\n\t%s=%s\n\t%s=%f\n",
			"ifile_name", ifile_name,
			"ofile_peak_trough_name", ofile_pt_name,
			"ofile_stride_name", ofile_st_name,
			"peak_threshold", pk_threshold
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


	// Filter troughs to filter out inappropriate values
	printf("Attempting to filter troughs.\n");
	S_i = (int *) malloc(sizeof(int) * n_T);
	temp = (double *)malloc(sizeof(double) * n_T);

	// Load all trough values into temp
	for(i = 0; i < n_T; i++){
		temp[i] = gyro_z[T_i[i]];
	}

	mean = calculate_mean(temp, n_T);

	for(i = 0; i < n_T; i++){
		if(gyro_z[T_i[i]] < mean){
			S_i[n_S] = T_i[i];
			n_S++;
		}
	}





	/*int idx_t;
	i = 0;
	while(i < n_T){
		idx_t = T_i[i];
		
		if(n_S==0){
			S_i[n_S] = T_i[i];
			i++;
			n_S++;
		}
		// if a trough is far enough from previous trough
		if(time[idx_t] - time[S_i[n_S - 1]] >= time_constraint){
			S_i[n_S] = T_i[i];
			i++;
			n_S++;
		}
		// a trough is too close to previous trough, skip it.
		else
			i++;
	}*/




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

	fprintf(fp, "S_i,S_t,S_x\n");
	for (i = 0; i < n_S; i++) {
		idx = S_i[i];
		fprintf(fp, "%d,%20.10lf,%lf\n",
				idx,
				time[idx],
				gyro_z[idx]);
	}
	fclose(fp);
	


	printf("stride_detector completed successfuly. Exiting.\n");

	return 0;
}