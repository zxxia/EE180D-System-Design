/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
/* for fabsf() */
#include <math.h>


//Find local max/min
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

//Extract all max/min data


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
	/*int *maxima_accel_y;
	int *minima_accel_y;
	int *maxima_gyro_y;
	int *minima_gyro_y;*/
	double* period;


	/*
	 * Check if the user entered the correct command line arguments
	 */
	if (argc != 5) {
		fprintf(stderr, "USEAGE: ./feature_detector <input data file> <input stride file> <output maxmin file> <output feature file>");
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





	return 0;
}