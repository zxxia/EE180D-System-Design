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
	char *numSamples;
	char *numInputNeurons;
	char *numOutputNeurons;
};
sig_atomic_t volatile run_flag = 1;

void generateHeader(FILE *fp_write, struct SampleInfo sampleInfo);
void writeFeatures(FILE *fp_read);
void writePatternVal(FILE *fp_write, char *pattern, const int numPatterns);
void do_when_interrupted(int sig);


int main(int argc, char **argv) {
	//Declaration
	const int numPatterns = 3;
	const char *targetName = "test_data.txt";
	FILE *fp_read;
	FILE *fp_write;
	struct SampleInfo sampleInfo;
	char *pattern;

	//Initialization
	if(argc != 6) {
		fprintf(stderr, "Usage: %s <read file_name> <pattern> <sample info>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, &do_when_interrupted);

	fp_read = fopen(argv[1], "r+");
	fp_write = fopen(targetName, "w+");

	if(!fp_write) {
		fprintf(stderr, "Fail to open file: test_data.txt\n");
		exit(EXIT_FAILURE);
	}
	if(!fp_read) {
		fprintf(stderr, "Fail to open file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	//Initialization
	pattern = argv[2];

	sampleInfo.numSamples = argv[3];
	sampleInfo.numInputNeurons = argv[4];
	sampleInfo.numOutputNeurons = argv[5];

	//Generate Header
	generateHeader(fp_write, sampleInfo);

	while(run_flag) {
		//Write features
		//Add pattern
		writePatternVal(fp_write, pattern, numPatterns);
		//Comment this out before using
		break;
	}

	fclose(fp_write);
	return 0;
}

//Generate Header: #samples in file, #input neuron, #output neuron
void generateHeader(FILE *fp_write, struct SampleInfo sampleInfo) {
	if ((fprintf(fp_write, "%s %s %s\n", sampleInfo.numSamples, 
		sampleInfo.numInputNeurons, sampleInfo.numOutputNeurons)) < 0) {
		fprintf(stderr, "Fail to generate header\n");
		exit(EXIT_FAILURE);
	}
	return;
}

//Calculate Sampling time

//write Pattern value
void writePatternVal(FILE *fp_write, char *pattern, const int numPatterns) {
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
			fprintf(fp_write, " ");
		}
		count++;
	}
	return ;
}

//Signal handler
void do_when_interrupted(int sig) {
	if (sig == SIGINT)
		run_flag = 0;
}

