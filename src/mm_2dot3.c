#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#include <sys/time.h>
#include <string.h>
#include <omp.h>

#define NUM_EVENTS 2
#define SIZE 1532

void fillMatrices (float **a, float **b, float **c ){
	for (unsigned i=0;i<SIZE;++i){
		for(unsigned j=0; j<SIZE; ++j){
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
	
	float **mat_a, **mat_b, **mat_c;
	mat_a = malloc (SIZE*sizeof(float*));
	mat_b = malloc (SIZE*sizeof(float*));
	mat_c = malloc (SIZE*sizeof(float*));
	for(unsigned i=0;i<SIZE;i++){
		mat_a[i]= malloc ( SIZE * sizeof (float*));
		mat_b[i]= malloc ( SIZE * sizeof (float*));
		mat_c[i]= malloc ( SIZE * sizeof (float*));
	}

	int Events[NUM_EVENTS]={PAPI_FP_INS,PAPI_TOT_CYC}; 

	int EventSet;
	long long values[NUM_EVENTS];

	//Fill Matrices
	fillMatrices(mat_a, mat_b, mat_c );

	int retval;
	retval = PAPI_library_init(PAPI_VER_CURRENT);
	retval = PAPI_create_eventset(&EventSet);
	retval = PAPI_add_events(EventSet,Events,NUM_EVENTS);

	retval = PAPI_start(EventSet);

	//Matrix Multiplication
	matrix_mult_2dot3 ( mat_a, mat_b, mat_c,SIZE);	

	retval = PAPI_stop(EventSet,values);

	for(int i = 0; i<NUM_EVENTS; i++){
		printf("%lld\n",values[i]);	
	}
	return 0;
}

