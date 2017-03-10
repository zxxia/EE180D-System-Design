#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>

/*******Input********/
/* 1. Raw Data file */
/* 2. Threshold     */
/* 3. Pattern number*/
/* 4. Sample number */
/********************/

/*******Output*******/
/* Training file    */
/********************/

/****************************NOTES****************************/
/* 	1. 	Pattern number should be the number of pattern we want
	  	to recognize. For this moment, pattern# = 3            
	2.	
*/
struct SampleInfo {
	int numSamples;
	char *numInputNeurons;
	char *numOutputNeurons;
};

/* max: takes a double array and return the max val in array*/
double max(double* array, int array_size);
/* normalize: takes a double array, array size, scale and normalize array*/
void normalize(double* array, int array_size, double scale);
int generateHeader(FILE *fp_write, struct SampleInfo sampleInfo);
void writePattern(FILE *fp_write, char *pattern, const int numPatterns);


int main(int argc, char **argv) {
	//Declaration
	int i;
	const int numPatterns = 3;
	FILE *fp;
	int rv;
	struct SampleInfo sampleInfo;
	char *pattern;

	ssize_t read; // number of lines read
	char *line = NULL;
	size_t len = 0;
	int N_SAMPLES;

	double* peak;
	double* trough;
	double* period;

	//Initialization
	if(argc != 6) {
		fprintf(stderr, "Usage: %s <read file_name> <output train file> <pattern> <input neuron num> <output neuron num>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}
//	signal(SIGINT, &do_when_interrupted);

	fprintf(stdout, "Open %s...\n", argv[1]);
	fp = fopen(argv[1], "r+");
	if(!fp) {
		fprintf(stderr, "Fail to open file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	// begin to read data from input data file
	fprintf(stdout, "Read data from  %s...\n", argv[1]);
	//discard header of file
	read = getline(&line, &len, fp);
	//count number of samples in input data file
	N_SAMPLES = 0;
	while((read = getline(&line, &len, fp)) != -1){
		N_SAMPLES++;
	}
	// go back to the beginning of the file
	rewind(fp);
	// discard header of file
	read = getline(&line, &len, fp);

	// declare arrays to hold all the data in input data file
	peak = (double*) malloc(sizeof(double) * N_SAMPLES);
	trough = (double*) malloc(sizeof(double) * N_SAMPLES);
	period = (double*) malloc(sizeof(double) * N_SAMPLES);

	i = 0;
	while((read = getline(&line, &len, fp)) != -1){
		// parse the line
		rv = sscanf(line, "%lf, %lf, %lf\n", &peak[i], &trough[i], &period[i]);
		if(rv != 3){
			fprintf(stderr, "%s %d \'%s\'. %s.\n", "Failed to read line", i, line, "Exiting");
			free(peak);
			free(trough);
			free(period);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	fclose(fp);
	fprintf(stdout, "Close %s...\n", argv[1]);
	// end read data from input data file

	// normailize all numbers
	fprintf(stdout, "Start to normalize data...\n");
	normalize(peak, N_SAMPLES, 4.0);
	normalize(trough, N_SAMPLES, 4.0);
	normalize(period, N_SAMPLES, max(period, N_SAMPLES));
	// normalization ends
	fprintf(stdout, "Stop normalizing data...\n");

	

	fprintf(stdout, "Open %s...\n", argv[2]);
	fp = fopen(argv[2], "w+");

	if(!fp) {
		fprintf(stderr, "Fail to open file: test_data.txt\n");
		free(peak);
		free(trough);
		free(period);
		exit(EXIT_FAILURE);
	}
	

	//Initialization
	pattern = argv[3];
	sampleInfo.numSamples = N_SAMPLES;
	sampleInfo.numInputNeurons = argv[4];
	sampleInfo.numOutputNeurons = argv[5];

	fprintf(stdout, "Write to %s...\n", argv[2]);

	//Generate Header
	if(generateHeader(fp, sampleInfo) == 0){
		free(peak);
		free(trough);
		free(period);
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	// Write Normalized  Feature and Pattern
	for(i = 0; i < N_SAMPLES; i++){
		fprintf(fp, "%lf\t%lf\t%lf\n", peak[i], trough[i], period[i]);
		writePattern(fp, pattern, numPatterns);
	}
	free(peak);
	free(trough);
	free(period);
	fprintf(stdout, "Close %s...\n", argv[2]);
	fclose(fp);
	return 0;
}


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
int generateHeader(FILE *fp_write, struct SampleInfo sampleInfo) {
	if ((fprintf(fp_write, "%d\t%s\t%s\n", sampleInfo.numSamples, 
		sampleInfo.numInputNeurons, sampleInfo.numOutputNeurons)) < 0) {
		fprintf(stderr, "Fail to generate header\n");
		return 0;
	}
	return 1;
}

//Calculate Sampling time

//write Pattern value
void writePattern(FILE *fp_write, char *pattern, const int numPatterns) {
	//Write pattern
	int count = 1;
	while(count <= numPatterns) {
		if (count == atoi(pattern)) {
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



