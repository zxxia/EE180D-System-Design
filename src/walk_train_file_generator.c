#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>


/* max: takes a double array and return the max val in array*/
double max(double* array, int array_size);
/* normalize: takes a double array, array size, scale and normalize array*/
void normalize(double* array, int array_size, double scale);
int generateHeader(FILE *fp_write, int numSamples, int numInputNeurons, int numOutputNeurons);
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

	double* var_accel_x_seg0;
	double* var_accel_x_seg1;
	double* var_accel_x_seg2;
	double* var_accel_x_seg3;
	double* period;
	int* pattern;

	//Initialization
	if(argc != 7) {
		fprintf(stderr, "Usage: ./%s <input data file 0> <input data file 1> <input data file 2> <output train file> <input neuron num> <output neuron num>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	inputNeuronNum = atoi(argv[argc-2]);
	outputNeuronNum = atoi(argv[argc-1]);
	outputFileName = argv[4];


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
		// go back to the beginning of the file
		rewind(fp);
		// discard header of file
		read = getline(&line, &len, fp);

		// allocate arrays to hold all the data in input data file
		if(i == 0){
			var_accel_x_seg0 = (double*) malloc(sizeof(double) * N_SAMPLES);
			var_accel_x_seg1 = (double*) malloc(sizeof(double) * N_SAMPLES);
			var_accel_x_seg2 = (double*) malloc(sizeof(double) * N_SAMPLES);
			var_accel_x_seg3 = (double*) malloc(sizeof(double) * N_SAMPLES);

			period = (double*) malloc(sizeof(double) * N_SAMPLES);
			pattern = (int*) malloc (sizeof(int) * N_SAMPLES);
		}
		else{
			var_accel_x_seg0 = (double*) realloc(var_accel_x_seg0, sizeof(double) * N_SAMPLES);
			var_accel_x_seg1 = (double*) realloc(var_accel_x_seg1, sizeof(double) * N_SAMPLES);
			var_accel_x_seg2 = (double*) realloc(var_accel_x_seg2, sizeof(double) * N_SAMPLES);
			var_accel_x_seg3 = (double*) realloc(var_accel_x_seg3, sizeof(double) * N_SAMPLES);
			
			period = (double*) realloc(period, sizeof(double) * N_SAMPLES);
			pattern = (int*) realloc(pattern, sizeof(int) * N_SAMPLES);
		}

		// begin to read data from input data file
		fprintf(stdout, "Read data from  %s...\n", argv[j]);

		while((read = getline(&line, &len, fp)) != -1){
			// parse the line
			pattern[i] = j;

			//rv = sscanf(line, "%lf,%lf,%lf,%lf,%lf\n", &max_accel_y[i], &min_accel_y[i], &max_gyro_y[i], &min_gyro_y[i], &period[i]);
			rv = sscanf(line, "%lf,%lf,%lf,%lf,%lf\n", 
				&var_accel_x_seg0[i], &var_accel_x_seg1[i],
				&var_accel_x_seg2[i], &var_accel_x_seg3[i],
				&period[i]);
			
			if(rv != 5){
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
	normalize(var_accel_x_seg0, N_SAMPLES, 6.0);
	normalize(var_accel_x_seg1, N_SAMPLES, 6.0);
	normalize(var_accel_x_seg2, N_SAMPLES, 6.0);
	normalize(var_accel_x_seg3, N_SAMPLES, 6.0);

	normalize(period, N_SAMPLES, max(period, N_SAMPLES));
	// normalization ends
	fprintf(stdout, "Stop normalizing data...\n");

	

	fprintf(stdout, "Open %s...\n", outputFileName);
	fp = fopen(outputFileName, "w+");

	if(!fp) {
		fprintf(stderr, "Fail to open file: test_data.txt\n");
		exit(EXIT_FAILURE);
	}
	

	fprintf(stdout, "Write to %s...\n", argv[5]);

	//Generate Header
	if(generateHeader(fp, N_SAMPLES, inputNeuronNum, outputNeuronNum) == 0){
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	// Write Normalized  Feature and Pattern
	for(i = 0; i < N_SAMPLES; i++){
		//fprintf(fp, "%lf\t%lf\t%lf\t%lf\t%lf\n", max_accel_y[i], min_accel_y[i], max_gyro_y[i], min_gyro_y[i],period[i]);
		fprintf(fp, "%lf\t%lf\t%lf\t%lf\t%lf\n", 
			var_accel_x_seg0[i], var_accel_x_seg1[i],
			var_accel_x_seg2[i], var_accel_x_seg3[i],
			period[i]);
		writePattern(fp, pattern[i], outputNeuronNum);
	}

	fprintf(stdout, "Close %s...\n", argv[2]);
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

//Generate Header: #samples in file, #input neuron, #output neuron
int generateHeader(FILE *fp_write, int numSamples, int numInputNeurons, int numOutputNeurons) {
	if ((fprintf(fp_write, "%d\t%d\t%d\n", numSamples, 
		numInputNeurons, numOutputNeurons)) < 0) {
		fprintf(stderr, "Fail to generate header\n");
		return 0;
	}
	return 1;
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



