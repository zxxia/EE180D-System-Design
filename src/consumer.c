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


#define BUFF_SIZE 		1024

sig_atomic_t volatile run_flag = 1;

static void do_when_interrupted(int signum) {
	printf("Catch a signal\n");
	run_flag = 0;
}


// delete this file so we don't process it again in the future
void clean_file(const char *fname)
{
	char delete_file[BUFF_SIZE];
	memset(delete_file, 0, BUFF_SIZE);
	sprintf(delete_file, "rm %s", fname);
	system(delete_file);
}


int count_samples(FILE *fp) {
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int count = 0;

	/* count the number of lines in the file */
	read = getline(&line, &len, fp); //discard header of file
	count = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		count++;
	}
	/* go back to the start of the file so that the data can be read */
	rewind(fp);
	return count;
}



int process_file(const char *fname) {
	//Generic variables
	int i = 0;
	int rv;

	//Variables for file
	
	FILE *fp;
	int fd;
	int n_samples;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	//Original Data Containers
	double *t = NULL; // sampling time
	double *accel_x = NULL, *accel_y = NULL, *accel_z = NULL;
	double *gyro_x = NULL, *gyro_y = NULL, *gyro_z = NULL;
	double t1, t2; // variable used to parse time_before and time_after
	double start_time; // variable used to compute sampling time

	
	int *S_i; 	// indicies of the start of each stride
	int n_S = 0; 	// number of strides

	// open the file to inspect
	fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file \'%s\'.\n", fname);
		return -1;;
	}

	// acquire the lock for the file (incase the producer is still writing to it)
	fd = fileno(fp);
	flock(fd, LOCK_EX);

	//Collect sample number
	n_samples = count_samples(fp);

	//Allocate original data containers
	t = (double*) malloc(sizeof(double) * n_samples);
	accel_x = (double*) malloc(sizeof(double) * n_samples);
	accel_y = (double*) malloc(sizeof(double) * n_samples);
	accel_z = (double*) malloc(sizeof(double) * n_samples);
	gyro_x = (double*) malloc(sizeof(double) * n_samples);
	gyro_y = (double*) malloc(sizeof(double) * n_samples);
	gyro_z = (double*) malloc(sizeof(double) * n_samples);
	
	
	
	// Collect raw data
	read = getline(&line, &len, fp); //discard header of file
	while ((read = getline(&line, &len, fp)) != -1) {
		/* parse the data */
		rv = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &t1, &t2, &accel_x[i], &accel_y[i], &accel_z[i], &gyro_x[i], &gyro_y[i], &gyro_z[i]);
		if (rv != 8) {
			fprintf(stderr, "Failed to read line %d \'%s\' in %s.\n", i,line, fname);
			//close the file to release the lock
			fclose(fp);
			return -1;
		}
		if(i == 0)
			start_time = t1;

		t[i] = (t1 + t2)/2.0 - start_time;
		i++;
	}

	//close the file to release the lock
	fclose(fp);

	

	//Stride detection
	S_i = (int *) malloc(sizeof(int) * n_samples);
	n_S = stride_detection(t, gyro_z, n_samples, S_i);
	if(n_S < 0){
		fprintf(stderr, "find_peaks_and_troughs failed\n");
		free(S_i);
		free(t);
		free(accel_x);
		free(accel_y);
		free(accel_z);
		free(gyro_x);
		free(gyro_y);
		free(gyro_z);
		return -1;
	}
	

	//Features needed
	GlobalFeature global_feature;
	TurnFeature turn_feature;
	WalkFeature walk_feature;
	StairFeature stair_feature;
	RunFeature run_feature;

	int pos[5];
	int motion_type;
	int turn_direction;
	int walk_speed;
	int stair_direction;
	int run_speed;

	for(i = 0; i < n_S - 1; i++){
		//Stride segmentation
		segmentation(pos, S_i[i], S_i[i+1]);
		//Extract global features
		extract_global_feature(&global_feature, pos, accel_y, gyro_y, t, gyro_z);
		//Obtain movement type
		motion_type = exe_global_neural_network(&global_feature);

		switch(motion_type){
			
			case TURN:
			extract_turn_feature(&turn_feature, pos, gyro_y, t);
			turn_direction = exe_turn_neural_network(&turn_feature);
			
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
			extract_walk_feature(&walk_feature, pos, accel_x, t);
			walk_speed = exe_walk_neural_network(&walk_feature);
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

			extract_stair_feature(&stair_feature, pos, accel_x, accel_y, t);
			stair_direction = exe_stair_neural_network(&stair_feature);
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
			extract_run_feature(&run_feature, pos, accel_x, t);
			run_speed = exe_run_neural_network(&run_feature);
			switch(run_speed){
				case SLOW_RUN:
				printf("Movement type is Slow Running\n");
				break;

				case FAST_RUN:
				printf("Movement type is Fast Running\n");
				break;
			}
			
			break;

			case SQUAT:
			printf("Movement type is Squating\n");
			break;
			
			case BACK_WALK:
			printf("Movement type is Backwards Walking\n");
			break;
		}

	}
	//Clean dynamically allocated memory
	free(S_i);
	free(t);
	free(accel_x);
	free(accel_y);
	free(accel_z);
	free(gyro_x);
	free(gyro_y);
	free(gyro_z);
	return 0;
}




int main(int argc, char **argv)
{	
	int rv;
	struct sigaction act;
	act.sa_handler = do_when_interrupted;
	rv = sigaction(SIGINT, &act, NULL);
	if(rv == -1){
		perror("ERROR: sigaction");
		exit(EXIT_FAILURE);
	}

	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char * fname;

	// Construct and initialize neuralnetworks	
	init_networks();

	while (run_flag) {
		rv = system("ls -1 file_*.csv 1>fnames.txt 2>/dev/null");
		if( WEXITSTATUS(rv) != 0){
			sleep(1);
			continue;
		}
		// open the file that contains the file names that we need to inspect
		fp = fopen("fnames.txt", "r");
		if (fp == NULL) {
			fprintf(stderr, "failed to open file\n");
			exit(EXIT_FAILURE);
		}
		while ((read = getline(&line, &len, fp)) != -1) {
			// strip newline from end of line read from file
			fname = line;
			fname[strlen(line)-1] = 0;
			// process file with fname
			process_file(fname);
			clean_file(fname);
		}
		// close the file that contains other filenames
		fclose(fp);
		// delete this file from the system
		system("rm fnames.txt");
	}
	// Destroy all neural networks
	destroy_networks();

	exit(EXIT_SUCCESS);
}
