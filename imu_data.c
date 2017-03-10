#include <stdio.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#define MILLION 1000000.0

/*float calculate_magnitude(data_t data) {
	return sqrt(data.x*data.x + data.y*data.y + data.z*data.z);
}*/
sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(int sig) {
	if (sig == SIGINT)
		run_flag = 0;
}

int main(int argc, char **argv) {
	FILE *fp;
	char *file_name;
	struct timeval start, end;
	double start_epoch, end_epoch;
	mraa_i2c_context accel, gyro;
	float a_res, g_res;
	accel_scale_t a_scale;
	gyro_scale_t g_scale;
	data_t ad, gd;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file_name>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, &do_when_interrupted);

	file_name = argv[1];
	fp = fopen(file_name, "w");
	if(fp==NULL) {
		fprintf(stderr, "Cannot open/create file %s/n",  file_name);
		return 1;
	}

	a_scale = A_SCALE_6G;
	g_scale = G_SCALE_500DPS;

	accel = accel_init();
	set_accel_ODR(accel, A_ODR_100);
	set_accel_scale(accel, a_scale);
	a_res = calc_accel_res(a_scale);

	gyro = gyro_init();
	set_gyro_ODR(gyro, G_ODR_190_BW_70);
	set_gyro_scale(gyro, g_scale);
	g_res = calc_gyro_res(g_scale);

	fprintf(fp, "timestamp_before, timestamp_after, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z\n");
	while(run_flag) {
		gettimeofday(&start, NULL);
		ad = read_accel(accel, a_res);
		gd = read_gyro(gyro, g_res);
		gettimeofday(&end, NULL);

		start_epoch = start.tv_sec + start.tv_usec / MILLION;
		end_epoch = end.tv_sec + end.tv_usec / MILLION;
		fprintf(fp, "%f, %f, %+f, %+f, %+f, %+f, %+f, %+f\n", start_epoch, end_epoch, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);
	
		usleep(100);
	}
	fclose(fp);
	return 0;
}
