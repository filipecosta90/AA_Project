#include <stdio.h>
#include <stdlib.h>
#include "papi.h"

#define NUM_EVENTS 8
#define MATRIX_SIZE 2048

int retval, EventSet = PAPI_NULL;
long_long values[NUM_EVENTS];

void clearCache(){
	double clearcache [30000000];
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
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
	float aux;
	for (unsigned i=0;i<N;i++){
		for(unsigned j=i;j<N;j++){
			aux = b[i][j];
			b[i][j]=b[j][i];
			b[j][i]=aux;
		}
	}
}


void matrix_mult_ijk (float **a, float **b, float **c, int N){
	transpose_matrix(b,N);
	for(unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			for(unsigned k=0;k<N;k++){
				c[i][j] += a[i][k] * b[j][k];
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

int events[1];
events[0]=PAPI_LD_INS;

        PAPI_start_counters(events,1);
	/*PAPI time measurement*/
	long_long start_us, stop_us;
	
	/*matrixes*/
	float **mat_a, **mat_b, **mat_c;
	mat_a = (float**) malloc (MATRIX_SIZE * sizeof(float*));
	mat_b = (float**) malloc (MATRIX_SIZE * sizeof(float*));
	mat_c = (float**) malloc (MATRIX_SIZE * sizeof(float*));

	for (unsigned i = 0; i < MATRIX_SIZE; ++i) {
		mat_a[i] = (float*) malloc (MATRIX_SIZE * sizeof(float)); 
		mat_b[i] = (float*) malloc (MATRIX_SIZE * sizeof(float)); 
		mat_c[i] = (float*) malloc (MATRIX_SIZE * sizeof(float)); 
	}

	/* Initialize the PAPI library */
	retval = PAPI_library_init(PAPI_VER_CURRENT);

	retval = PAPI_create_eventset(&EventSet);
	/* Add Total Instructions Executed to our EventSet */
	retval = PAPI_add_event(EventSet, PAPI_FP_OPS);
	retval = PAPI_add_event(EventSet, PAPI_FP_INS);
	retval = PAPI_add_event(EventSet, PAPI_TOT_CYC);
	retval = PAPI_add_event(EventSet, PAPI_TOT_INS);
	retval = PAPI_add_event(EventSet, PAPI_LD_INS);
	retval = PAPI_add_event(EventSet, PAPI_L1_DCM);
	retval = PAPI_add_event(EventSet, PAPI_L2_DCM);
	retval = PAPI_add_event(EventSet, PAPI_L3_TCM);

	//Fill Matrices
	fillMatrices(mat_a, mat_b, mat_c, MATRIX_SIZE );
	printf("filled matrix\n");
	clearCache();
	printf("cleared chache\n");
	start_us = PAPI_get_real_usec();
	printf("starting\n");
	retval = PAPI_start(EventSet);
	printf("papi started \n");
//Matrix Multiplication


     PAPI_start_counters(events,1);
int x = 0;
for ( x = 0; x <10000; x++ ){
x = x+1;
}

        PAPI_stop_counters(counts,1);
  retval = PAPI_library_init(PAPI_VER_CURRENT);


	matrix_mult_ikj ( mat_a, mat_b, mat_c, MATRIX_SIZE);	

        PAPI_read(EventSet, values);

	retval = PAPI_stop(EventSet,values);

	stop_us = PAPI_get_real_usec();
	printf("stoping\n");
	FILE *file;
	file = fopen(argv[1],"a");
	long_long duration_us = stop_us - start_us;
	
	printf("%lld\n", duration_us );
for(int i = 0; i<NUM_EVENTS; i++){
		fprintf(file,"%lld,", values[i]);	
		printf("%lld\t", values[i]);	
	}
	fclose(file);
	return 0;
}

