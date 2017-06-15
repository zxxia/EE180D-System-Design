#include "stride_detector.h"

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

int find_peaks_and_troughs(
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

int stride_detection(double *t, double *gyro_z, int n_samples, int* S_i)
{
	int rv;
	//Peak and trough variables
	int *P_i; 	// indicies of each peak found by peak detection
	int *T_i; 	// indicies of each trough found by trough detection
	int n_P; 	// number of peaks
	int n_T; 	// number of troughs
	int n_S;
	int i;

	double *temp;
	double mean_val;
	double std_val;
	double std_mean_ratio;

	const float pk_threshold = 50.0;

	P_i = (int *) malloc(sizeof(int) * n_samples);
	T_i = (int *) malloc(sizeof(int) * n_samples);

	//Find peak and trough
	rv = find_peaks_and_troughs(
			gyro_z, n_samples, 
			pk_threshold, P_i, T_i, 
			&n_P, &n_T);

	if (rv < 0){
		free(P_i);
		free(T_i);
		return -1;
	}

	
	temp = (double *)malloc(sizeof(double) * n_P);

	for(i = 0; i < n_P; i++){
		temp[i] = gyro_z[P_i[i]];
	}
	mean(temp, 0, n_P, &mean_val);
	std(temp, 0, n_P, &std_val);
	std_mean_ratio = std_val/mean_val;
	n_S = 0;
	for(i = 0; i < n_P; i++){
		if(std_mean_ratio < 0.5 || gyro_z[P_i[i]] > mean_val){
			if(n_S == 0 || t[P_i[i]] - t[S_i[n_S-1]] > 0.5){
				S_i[n_S] = P_i[i];
				n_S++;
			}
		}
	}

	free(temp);
	free(P_i);
	free(T_i);

	return n_S;
}

