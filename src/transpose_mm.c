#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#include <sys/time.h>
#include <string.h>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <iostream>

#define NUM_EVENTS 7
#define TIME_RESOLUTION 1000000

long long unsigned initial_time, final_time, total_duration;
timeval t;

using namespace std;

void clearCache(){
	double clearcache [30000000];
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}

void start (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

void stop ( void ) {
	gettimeofday(&t, NULL);
	final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
	total_duration = final_time - initial_time;
}

void fillMatrices (float **a, float **b, float **c, int N){
	for (unsigned i=0;i<N;++i){
		for(unsigned j=0; j<N; ++j){
			a[i][j] = ((float) rand()) / ((float) RAND_MAX);
			b[i][j] = ((float) rand()) / ((float) RAND_MAX);
			c[i][j] = 0;
		}
	}
}

void transpose_matrix (float **b, int N){
	float transpose[N][N];
	for (unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			transpose[j][i] = b[i][j];
		}
	}
	for (unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			b[i][j]=transpose[i][j];
		}
	}
}

void matrix_mult_ijk (float **a, float **b, float **c, int N){
	transpose_matrix(b,N);
	for(unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			for(unsigned k=0;k<N;k++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void matrix_mult_ikj (float **a, float **b, float **c, int N){
	for(unsigned i=0;i<N;i++){
		for(unsigned k=0;k<N;k++){
			for(unsigned j=0;j<N;j++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

int main (int argc, char *argv[]){

	int  size = atoi(argv[1]);	

	float **mat_a, **mat_b, **mat_c;
	mat_a = new float*[size];
	mat_b = new float*[size];
	mat_c = new float*[size];

	for (unsigned i = 0; i < size; ++i) {
		mat_a[i] = new float[size];
		mat_b[i] = new float[size];
		mat_c[i] = new float[size];
	}


	int Events[NUM_EVENTS]={PAPI_FP_OPS,PAPI_TOT_CYC,PAPI_TOT_INS,PAPI_LD_INS,PAPI_L1_TCM,PAPI_L2_TCM,PAPI_L3_TCM}; 

	int EventSet;
	long long values[NUM_EVENTS];

	//Fill Matrices
	fillMatrices(mat_a, mat_b, mat_c, size );

	int retval;
	retval = PAPI_library_init(PAPI_VER_CURRENT);
	retval = PAPI_create_eventset(&EventSet);
	retval = PAPI_add_events(EventSet,Events,NUM_EVENTS);
		clearCache();
	retval = PAPI_start(EventSet);
	start();
	//Matrix Multiplication
	matrix_mult_ikj ( mat_a, mat_b, mat_c, size);	
	stop();
	retval = PAPI_stop(EventSet,values);
	
	FILE *file;
	file = fopen(argv[1],"a");

	fprintf(file,"%lld,",(float)total_duration);
	for(int i = 0; i<NUM_EVENTS; i++){
		fprintf(file,"%f,",(float) values[i]);	
		printf("%f\t",(float) values[i]);	
	}
	fprintf(file,"%f\n",(float)values[NUM_EVENTS-1]);
	printf("%f\n", (float) values[NUM_EVENTS-1]);
	fclose(file);
	return 0;
}

