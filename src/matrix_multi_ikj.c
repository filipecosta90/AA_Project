#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#include <sys/time.h>
#include <string.h>
#include <omp.h>

#define NUM_EVENTS 2
#define SIZE 1532

float a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];

void fillMatrices (void){
	for (unsigned i=0;i<SIZE;++i){
		for(unsigned j=0; j<SIZE; ++j){
			a[i][j] = ((float) rand()) / ((float) RAND_MAX);
			b[i][j] = 1;
			c[i][j] = 0;
		}
	}
}

void matrix_multiplication(float a[][SIZE], float b[][SIZE], float c[][SIZE], int N){
	for(unsigned i=0;i<N;i++){
		for(unsigned k=0;k<N;k++){
			for(unsigned j=0;j<N;j++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

int main (int argc, char *argv[]){

	

	int Events[NUM_EVENTS]={PAPI_FP_INS,PAPI_TOT_CYC}; 
	int EventSet;
	long long values[NUM_EVENTS];
	
	fillMatrices();

	int retval;
	retval = PAPI_library_init(PAPI_VER_CURRENT);
	retval = PAPI_create_eventset(&EventSet);
	retval = PAPI_add_events(EventSet,Events,NUM_EVENTS);

	retval = PAPI_start(EventSet);
	//Matrix Multiplicationi
	matrix_multiplication(&a,&b,&c,SIZE);	

	retval = PAPI_stop(EventSet,values);

	for(int i = 0; i<NUM_EVENTS; i++){
		printf("%ll\n",values[i]);	
	}

	return 1;
}

