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

void matrix_mult_2dot3 (float **a, float **b, float **c, int N){
	for(unsigned i=0;i<N;i++){
		for(unsigned k=0;k<N;k++){
			for(unsigned j=0;j<N;j++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

int main (int argc, char *argv[]){
	
	int size = atoi(argv[1]);	

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
	int ite = 0;
	FILE *file;
	file = fopen("../Results/results_L1.csv","w");
	fprintf(file,"Execution Time,PAPI_FP_OPS,PAPI_TOT_CYC,PAPI_TOT_INS,PAPI_LD_INS,PAPI_L1_TCM,PAPI_L2_TCM,PAPI_L3_TCM\n");
	do{
		clearCache();
		retval = PAPI_start(EventSet);
		start();
		//Matrix Multiplication
		matrix_mult_2dot3 ( mat_a, mat_b, mat_c, size);	
		stop();
		retval = PAPI_stop(EventSet,values);
		
		fprintf(file,"%d,",total_duration);
		for(int i = 0; i<NUM_EVENTS-1; i++){
			fprintf(file,"%lld,",values[i]);	
		}
		fprintf(file,"%lld\n",values[NUM_EVENTS-1]);
		
		retval = PAPI_reset(EventSet);		
	
		ite++;

	}while(ite<8);

	fclose(file);

	return 0;
}

