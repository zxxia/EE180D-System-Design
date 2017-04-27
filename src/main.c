/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>


#include "feature_detector.h"
#include "stride_detector.h"
#include "math_func.h"

#define BUFF_SIZE 1024


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
	char* ofile_feature_name; // feature file(max min period)

	
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
	double mean_val;

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
	if (argc != 7) {
		fprintf(stderr, "USEAGE: ./main <input data file> <output peak trough detection file> <output stride detection file> <output maxmin file> <output feature file> <threshold>\n");
		exit(EXIT_FAILURE);

	} else {
		ifile_name = argv[1];
		ofile_pt_name = argv[2];
		ofile_st_name = argv[3];
		
		ofile_maxmin_name = argv[4];
		ofile_feature_name = argv[5];

		pk_threshold = atof(argv[6]);
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


	// Filter troughs to filter out inappropriate values
	printf("Attempting to filter troughs.\n");
	S_i = (int *) malloc(sizeof(int) * n_P);
	temp = (double *)malloc(sizeof(double) * n_P);

	// Load all trough values into temp
	for(i = 0; i < n_P; i++){
		temp[i] = gyro_z[P_i[i]];
	}

	mean(temp, 0, n_P, &mean_val);

	for(i = 0; i < n_P; i++){
		if(gyro_z[P_i[i]] > mean_val){
			S_i[n_S] = P_i[i];
			n_S++;
		}
	}

	global_feature(accel_y, gyro_y, time, S_i, n_S, ofile_maxmin_name, ofile_feature_name);

	//


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
