#include "math_func.h"

// Maximum
void max(double* data, int start_pos, int end_pos, double* max_val)
{
	int i;
	*max_val = data[start_pos];
	for(i = start_pos; i < end_pos; i++){
		if(data[i] > *max_val)
			*max_val = data[i];
	}
}

// Minimum
void min(double* data, int start_pos, int end_pos, double* min_val)
{
	int i;
	*min_val = data[start_pos];
	for(i = start_pos; i < end_pos; i++){
		if(data[i] < *min_val)
			*min_val = data[i];
	}
}

//Mean
void mean(double* data, int start_pos, int end_pos, double* mean_val)
{
	int i;
	int totalPos = end_pos - start_pos;
	double sum = 0;
	for(i = start_pos; i < end_pos; i++) {
		sum = sum + data[i];
	}
	*mean_val = sum / (double)totalPos;
}

//Variance
void variance(double* data, int start_pos, int end_pos, double* var)
{
	int i;
	int totalPos = end_pos - start_pos;
	double mean_val, sum = 0;
	mean(data, start_pos, end_pos, &mean_val);
	for(i = start_pos; i < end_pos; i++) {
		sum = sum + pow((data[i] - mean_val), 2);
	}
	*var = sum / (double)totalPos;
}

//Standard deviation
void std(double* data, int start_pos, int end_pos, double* std_val)
{
	double var;
	variance(data, start_pos, end_pos, &var);
	*std_val = sqrt(var);
}

//Root-mean-square
void rms(double* data, int start_pos, int end_pos, double* rms_val)
{
	int i;
	int totalPos = end_pos - start_pos;
	double sum = 0;
	for(i = start_pos; i < end_pos; i++) {
		sum = sum + pow(data[i], 2);
	}
	*rms_val = pow((sum / (double)totalPos), 0.5);
}

//Mean absolute deviation
void mad(double* data, int start_pos, int end_pos, double* mad_val)
{
	int i;
	int totalPos = end_pos - start_pos;
	double mean_val, sum = 0;
	mean(data, start_pos, end_pos, &mean_val);
	for (i = start_pos; i < end_pos; i++) {
		sum = sum + fabs(data[i] - mean_val);
	}
	*mad_val = sum / (double)totalPos;
}


//Numerical Integration using Trapezoid Method
void integral(double* data, double* time, int start_pos, int end_pos, double* integral_val)
{
	int i;
	*integral_val = 0.0;
	for(i = start_pos; i < end_pos-1; i++){
		*integral_val += 0.5 * (data[i + 1] + data[i]) * (time[i + 1] - time[i]);
	}
}


// Kurtosis
void kurtosis(double* data, int start_pos, int end_pos, double* kurt_val)
{
	int i;
	double std_val;
	double mean_val;
	double miu4 = 0.0; //Fourth central moment
	mean(data, start_pos, end_pos, &mean_val);
	std(data, start_pos, end_pos, &std_val);
	*kurt_val = 0.0;
	for(i = start_pos; i < end_pos; i++)
		miu4 += pow(data[i] - mean_val, 4) / (end_pos - start_pos);
	*kurt_val = miu4/pow(std_val, 4);
}

void skewness(double* data, int start_pos, int end_pos, double* skew_val)
{
	int i;
	double std_val;
	double mean_val;
	double miu3 = 0.0; //Third central moment
	mean(data, start_pos, end_pos, &mean_val);
	std(data, start_pos, end_pos, &std_val);
	*skew_val = 0.0;
	for(i = start_pos; i < end_pos; i++)
		miu3 += pow(data[i] - mean_val, 3) / (end_pos - start_pos);
	*skew_val = miu3/pow(std_val, 3);

}

//Max-min ratio
//Skewness?????Not a good one????
//Kurtosis?????Consider for later
//Correlation??????????
