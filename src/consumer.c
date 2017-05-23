/* for file and terminal I/O */
#include <stdio.h>
/* for string manip */
#include <string.h>
/* for exit() */
#include <stdlib.h>
#include <sys/file.h>
#include <signal.h>

#include "feature_detector.h"
#include "stride_detector.h"
#include "math_func.h"
#include "exe_neural_network.h"


#define BUFF_SIZE 1024

sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(int sig) {
	if (sig == SIGINT)
		run_flag = 0;
}

void clean_file(FILE *fp, const char *fname)
{
	char delete_file[1024];
	memset(delete_file, 0, 1024);
	//close the file to release the lock
	if(fp != NULL)
		fclose(fp);
	// delete this file so we don't process it again in the future
	sprintf(delete_file, "rm %s", fname);
	system(delete_file);
}


int count_samples(FILE *fp) {
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int N_SAMPLES = 0;

	if (fp == NULL) {
		sleep(1);
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
	return N_SAMPLES;
}



int process_file(const char *fname, float pk_threshold) {
	//Generic variables
	int i = 0;
	int rv;

	//Variables for file
	
	FILE *fp;
	int fd;
	int N_SAMPLES;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	//Original Data Containers
	double *time;
	double *accel_x, *accel_y, *accel_z;
	double *gyro_x, *gyro_y, *gyro_z;
	double t1, t2; // variable used to parse time_before and time_after
	double start_time; // variable used to computer sampling time

	
	int *S_i; 	// indicies of the start of each stride
	int n_S = 0; 	// number of strides

	// open the file to inspect
	//printf("\tOpening file \'%s\'\n", fname);
	fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file \'%s\'.\n", fname);
		clean_file(fp, fname);
		return -1;;
	}

	// acquire the lock for the file (incase the producer is still writing to it)
	fd = fileno(fp);
	flock(fd, LOCK_EX);

	//Collect sample number
	N_SAMPLES = count_samples(fp);

	//Allocate original data containers
	time = (double*) malloc(sizeof(double) * N_SAMPLES);
	accel_x = (double*) malloc(sizeof(double) * N_SAMPLES);
	accel_y = (double*) malloc(sizeof(double) * N_SAMPLES);
	accel_z = (double*) malloc(sizeof(double) * N_SAMPLES);
	gyro_x = (double*) malloc(sizeof(double) * N_SAMPLES);
	gyro_y = (double*) malloc(sizeof(double) * N_SAMPLES);
	gyro_z = (double*) malloc(sizeof(double) * N_SAMPLES);
	
	
	
	// Collect raw data
	//printf("\tAcquired lock. Printing file contents of \'%s\':\n", fname);
	read = getline(&line, &len, fp); //discard header of file
	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &t1, &t2, &accel_x[i], &accel_y[i], &accel_z[i], &gyro_x[i], &gyro_y[i], &gyro_z[i]);
		if (rv != 8) {
			fprintf(stderr, "%s %d \'%s\'.\n",
					"Failed to read line",
					i,line);
			clean_file(fp, fname);
			return -1;
		}
		if(i == 0)
			start_time = t1;

		time[i] = (t1 + t2)/2.0 - start_time;
		i++;
	}

//	clean_file(fp, fname);

	

	//Stride detection
	//printf("Attempting to do stride detection.\n");
	S_i = (int *) malloc(sizeof(int) * N_SAMPLES);
	n_S = stride_detection(gyro_z, N_SAMPLES, pk_threshold, S_i);
	if(n_S < 0){
		fprintf(stderr, "find_peaks_and_troughs failed\n");
		return -1;
	}
	

	//Collect features
	//printf("Attempting to collect features for neural networks...\n");
	GlobalFeature* global_feature = (GlobalFeature*) malloc(sizeof(GlobalFeature) * n_S-1);
	TurnFeature* turn_feature = (TurnFeature*) malloc(sizeof(TurnFeature) * n_S-1);
	WalkFeature* walk_feature = (WalkFeature*) malloc(sizeof(WalkFeature) * n_S-1);
	StairFeature* stair_feature = (StairFeature*) malloc(sizeof(StairFeature) * n_S-1);

	int pos[5];
	for(i = 0; i< n_S-1; i++){
		//segmentation
		segmentation(pos, S_i[i], S_i[i+1]);
		//Extract features
		extract_global_feature(&global_feature[i], pos, accel_y, gyro_y, time);
		extract_turn_feature(&turn_feature[i], pos, gyro_y, time);
		extract_walk_feature(&walk_feature[i], pos, accel_x, time);
		extract_stair_feature(&stair_feature[i], pos, accel_x, accel_y, time);
	}


	//Execute neural network
	int motion_type;
	int turn_direction;
	int walk_speed;
	int stair_direction;

	for(i = 0; i < n_S - 1; i++){
		motion_type = exe_global_neural_network(&global_feature[i]);

		switch(motion_type){
			
			case TURN:
			turn_direction = exe_turn_neural_network(&turn_feature[i]);
			
			switch(turn_direction){
				case LEFT_TURN:
				printf("Movement type is Left Turning\n");
				break;
			
				case RIGHT_TURN:
				printf("Movement type is Right Turning\n");
				break;
			}
			break;
			
			
			case WALK:
			walk_speed = exe_walk_neural_network(&walk_feature[i]);
			switch (walk_speed){
				case SLOW_WALK:
				printf("Movement type is Slow Walking\n");
				break;
				case MED_WALK:
				printf("Movement type is Medium Walking\n");
				break;
				case FAST_WALK:
				printf("Movement type is Fast Walking\n");
				break;
			}
			break;
			
			
			case STAIR:
			stair_direction = exe_stair_neural_network(&stair_feature[i]);
			switch(stair_direction){
				case UP_STAIR:
				printf("Movement type is Stairs Up\n");
				break;

				case DOWN_STAIR:
				printf("Movement type is Stairs Down\n");
				break;
			}
			break;
			
			
			case RUN:
			printf("Movement type is Running\n");
			break;
		}

	}


	return 1;
}




int main(int argc, char **argv)
{	
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char * fname;
	float pk_threshold;

	pk_threshold = atof(argv[1]);
	// Construct and initialize neuralnetworks	
	init_networks();

	while (run_flag) {
		system("ls -1 file_*.csv > fnames.txt");
		// open the file that contains the file names that we need to inspect
		fp = fopen("fnames.txt", "r");
		if (fp == NULL) {
			fprintf(stderr, "failed to open file\n");
			exit(EXIT_FAILURE);
		}
		while ((read = getline(&line, &len, fp)) != -1) {
			//printf("Discovered file \'%s\'", line);
			// strip newline from end of line read from file
			fname = line;
			fname[strlen(line)-1] = 0;
			// process file with fname
			//printf("Processing file \'%s\'", fname);
			process_file(fname, pk_threshold);
			//usleep(200);

		}
		// close the file that contains other filenames
		fclose(fp);
		// delete this file from the system
		//system("rm fnames.txt");
		//sleep(1);
	}
	// Destroy all neural networks
	destroy_networks();

	return EXIT_SUCCESS;
}
