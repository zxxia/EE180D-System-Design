#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>


/* max: takes a double array and return the max val in array*/
double max(double* array, int array_size);
/* normalize: takes a double array, array size, scale and normalize array*/
void normalize(double* array, int array_size, double scale);

void writePattern(FILE *fp_write, int pattern, const int numPatterns);


int main(int argc, char **argv) {
	//Declaration
	int i = 0;
	int j;
	FILE *fp;
	int rv;


	ssize_t read; // number of lines read
	char *line = NULL;
	size_t len = 0;
	int N_SAMPLES = 0;

	char* outputFileName;
	int inputNeuronNum;
	int outputNeuronNum;

	double *max_accel_y_seg0, *min_accel_y_seg0;
	double *max_accel_y_seg1, *min_accel_y_seg1;
	double *max_accel_y_seg2, *min_accel_y_seg2;
	double *max_accel_y_seg3, *min_accel_y_seg3;

	double *max_gyro_y_seg0, *min_gyro_y_seg0;
	double *max_gyro_y_seg1, *min_gyro_y_seg1;
	double *max_gyro_y_seg2, *min_gyro_y_seg2;
	double *max_gyro_y_seg3, *min_gyro_y_seg3;
	double *abs_mean;
	double *period;
	int *pattern;

	//Initialization
	if(argc != 8) {
		fprintf(stderr, "Usage: %s <turn feature file> <walk feature file> <stairs feature file 2> <run feature file> <output train file> <input neuron num> <output neuron num>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	inputNeuronNum = atoi(argv[argc-2]);
	outputNeuronNum = atoi(argv[argc-1]);
	outputFileName = argv[argc-3];


	// Open each input feature file and combine them into one train file
	for(j = 1; j <= outputNeuronNum; j++){
		fprintf(stdout, "Open %s...\n", argv[j]);
		fp = fopen(argv[j], "r+");
		if(!fp) {
			fprintf(stderr, "Fail to open file: %s\n", argv[j]);
			exit(EXIT_FAILURE);
		}
	
		//discard header of file
		read = getline(&line, &len, fp);
		//count number of samples in input data file
		while((read = getline(&line, &len, fp)) != -1){
			N_SAMPLES++;
		}
		fclose(fp);
		fprintf(stdout, "Close %s...\n", argv[j]);
	}
	max_accel_y_seg0 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_accel_y_seg0 = (double*) malloc(sizeof(double) * N_SAMPLES);
	
	max_accel_y_seg1 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_accel_y_seg1 = (double*) malloc(sizeof(double) * N_SAMPLES);
	
	max_accel_y_seg2 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_accel_y_seg2 = (double*) malloc(sizeof(double) * N_SAMPLES);
			
	max_accel_y_seg3 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_accel_y_seg3 = (double*) malloc(sizeof(double) * N_SAMPLES);

	max_gyro_y_seg0 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_gyro_y_seg0 = (double*) malloc(sizeof(double) * N_SAMPLES);
	
	
	max_gyro_y_seg1 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_gyro_y_seg1 = (double*) malloc(sizeof(double) * N_SAMPLES);

	
	max_gyro_y_seg2 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_gyro_y_seg2 = (double*) malloc(sizeof(double) * N_SAMPLES);

	
	max_gyro_y_seg3 = (double*) malloc(sizeof(double) * N_SAMPLES);
	min_gyro_y_seg3 = (double*) malloc(sizeof(double) * N_SAMPLES);

	
	abs_mean = (double*) malloc(sizeof(double) * N_SAMPLES);
	period = (double*) malloc(sizeof(double) * N_SAMPLES);
	pattern = (int*) malloc (sizeof(int) * N_SAMPLES);

	for(j = 1; j<= outputNeuronNum; j++){
		fprintf(stdout, "Open %s...\n", argv[j]);
		fp = fopen(argv[j], "r+");
		if(!fp) {
			fprintf(stderr, "Fail to open file: %s\n", argv[j]);
			exit(EXIT_FAILURE);
		}
		//discard header of file
		read = getline(&line, &len, fp);

		// begin to read data from input data file
		fprintf(stdout, "Read data from  %s...\n", argv[j]);

		// parse the line
		while((read = getline(&line, &len, fp)) != -1){
			pattern[i] = j;

			rv = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", 
				&max_accel_y_seg0[i], &min_accel_y_seg0[i],
				&max_accel_y_seg1[i], &min_accel_y_seg1[i],
				&max_accel_y_seg2[i], &min_accel_y_seg2[i],
				&max_accel_y_seg3[i], &min_accel_y_seg3[i],
				&max_gyro_y_seg0[i], &min_gyro_y_seg0[i],
				&max_gyro_y_seg1[i], &min_gyro_y_seg1[i],
				&max_gyro_y_seg2[i], &min_gyro_y_seg2[i],
				&max_gyro_y_seg3[i], &min_gyro_y_seg3[i],
				&abs_mean[i],
				&period[i]);
			
			if(rv != 18){
				fprintf(stderr, "%s %d \'%s\'. %s.\n", "Failed to read line", i, line, "Exiting");
				exit(EXIT_FAILURE);
			}
			i++;
		}

		fclose(fp);
		fprintf(stdout, "Close %s...\n", argv[j]);
		// end read data from input data file
	}

	// normailize all numbers
	fprintf(stdout, "Start to normalize data...\n");
	normalize(max_accel_y_seg0, N_SAMPLES, 6.0);
	normalize(min_accel_y_seg0, N_SAMPLES, 6.0);
	normalize(max_accel_y_seg1, N_SAMPLES, 6.0);
	normalize(min_accel_y_seg1, N_SAMPLES, 6.0);
	normalize(max_accel_y_seg2, N_SAMPLES, 6.0);
	normalize(min_accel_y_seg2, N_SAMPLES, 6.0);
	normalize(max_accel_y_seg3, N_SAMPLES, 6.0);
	normalize(min_accel_y_seg3, N_SAMPLES, 6.0);

	normalize(max_gyro_y_seg0, N_SAMPLES, 500.0);
	normalize(min_gyro_y_seg0, N_SAMPLES, 500.0);

	normalize(max_gyro_y_seg1, N_SAMPLES, 500.0);
	normalize(min_gyro_y_seg1, N_SAMPLES, 500.0);

	normalize(max_gyro_y_seg2, N_SAMPLES, 500.0);
	normalize(min_gyro_y_seg2, N_SAMPLES, 500.0);

	normalize(max_gyro_y_seg3, N_SAMPLES, 500.0);
	normalize(min_gyro_y_seg3, N_SAMPLES, 500.0);

	normalize(abs_mean, N_SAMPLES, max(abs_mean, N_SAMPLES));
	normalize(period, N_SAMPLES, max(period, N_SAMPLES));
	// normalization ends
	fprintf(stdout, "Stop normalizing data...\n");

	

	fprintf(stdout, "Open %s...\n", outputFileName);
	fp = fopen(outputFileName, "w");

	if(!fp) {
		fprintf(stderr, "Fail to open file: test_data.txt\n");
		exit(EXIT_FAILURE);
	}
	

	fprintf(stdout, "Write to %s...\n", outputFileName);

	//Generate Header
	fprintf(fp, "%d\t%d\t%d\n", N_SAMPLES, inputNeuronNum, outputNeuronNum);

	// Write Normalized  Feature and Pattern
	for(i = 0; i < N_SAMPLES; i++){
		
		fprintf(fp, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", 
			max_accel_y_seg0[i], min_accel_y_seg0[i],
			max_accel_y_seg1[i], min_accel_y_seg1[i],
			max_accel_y_seg2[i], min_accel_y_seg2[i],
			max_accel_y_seg3[i], min_accel_y_seg3[i],
			max_gyro_y_seg0[i], min_gyro_y_seg0[i],
			max_gyro_y_seg1[i], min_gyro_y_seg1[i],
			max_gyro_y_seg2[i], min_gyro_y_seg2[i],
			max_gyro_y_seg3[i], min_gyro_y_seg3[i],
			abs_mean[i], period[i]);
		writePattern(fp, pattern[i], outputNeuronNum);
	}

	fprintf(stdout, "Close %s...\n", outputFileName);
	fclose(fp);
	return 0;
}

// Find max in array
double max(double*array, int array_size){
	double max_val = 0.0;
	double tmp;
	int i;
	for(i = 0; i < array_size; i++){
		tmp = fabs(array[i]);
		if(tmp > max_val)
			max_val = tmp;
	}
	return max_val;
}


// Normalize all input so that it can be feeded into neuronetwork
void normalize(double* array, int array_size, double scale){
	int i;
	for( i = 0; i < array_size; i++){
		array[i] = array[i]/scale;
	}
}



//write Pattern value
void writePattern(FILE *fp_write, int pattern, const int numPatterns) {
	//Write pattern
	int count = 1;
	while(count <= numPatterns) {
		if (count == pattern) {
			fprintf(fp_write, "1");
		} else {
			fprintf(fp_write, "-1");
		}

		if(count == numPatterns) {
		fprintf(fp_write, "\n");
		} else {
			fprintf(fp_write, "\t");
		}
		count++;
	}
	return;
}



