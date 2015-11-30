#include <stdio.h>
#include <stdlib.h>
#include "papi.h"

#define NUM_EVENTS 5
#define CLEAR 4194304

void clearCache(){
	double clearcache [CLEAR];
	for (unsigned i = 0; i < CLEAR; ++i)
		clearcache[i] = i;
}

void fillMatrices (float **a, float **b, float **c, int N){
	for (unsigned i=0;i<N;++i){
		for(unsigned j=0; j<N; ++j){
			a[i][j] = ((float) rand()) / ((float) RAND_MAX);
			b[i][j] = ((float) rand()) / ((float) RAND_MAX);
			c[i][j] = 0.0;
		}
	}
}

void clearMatrix ( float **c, int N){
	for (unsigned i=0;i<N;++i){
		for(unsigned j=0; j<N; ++j){
			c[i][j] = 0.0;
		}
	}
}

void fillMatrices_232 (float **a, float **b, float **c, int N){
	for (unsigned i=0;i<N;++i){
		for(unsigned j=0; j<N; ++j){
			a[i][j] = ((float) rand()) / ((float) RAND_MAX);
			b[i][j] = 1.0;
			c[i][j] = 0.0;
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


void fprint_matrix (FILE* fp, float **b, int N){
	for (unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			fprintf(fp,"%f,",b[i][j]);
		}
		fprintf(fp,"\n");
	}
}

void print_matrix ( float **b, int N){
	for (unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			printf("%f\t",b[i][j]);
		}
		printf("\n");
	}
}

void matrix_mult_ijk (float **a, float **b, float **c, int N){
	for(unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;j++){
			for(unsigned k=0;k<N;k++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void matrix_mult_ijk_transposed (float **a, float **b, float **c, int N){
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

	int size = atoi (argv[1]);

	/*PAPI time measurement*/
	long_long start_us, stop_us;

	/*matrixes*/
	float **mat_a, **mat_b, **mat_c;
	mat_a = (float**) malloc (size * sizeof(float*));
	mat_b = (float**) malloc (size * sizeof(float*));
	mat_c = (float**) malloc (size * sizeof(float*));

	for (unsigned i = 0; i < size; ++i) {
		mat_a[i] = (float*) malloc (size * sizeof(float)); 
		mat_b[i] = (float*) malloc (size * sizeof(float)); 
		mat_c[i] = (float*) malloc (size * sizeof(float)); 
	}

	//Fill Matrices
	fillMatrices(mat_a, mat_b, mat_c, size );
//	clearCache();

	/* PAPI VARIABLES */
	int Events[NUM_EVENTS]={PAPI_FP_OPS,PAPI_SP_OPS,PAPI_DP_OPS,PAPI_VEC_SP,PAPI_VEC_DP};
	int EventSet;	
long long counts[NUM_EVENTS];

	/* FLOATING POINT OPERATIONS */
/*	events[1]=PAPI_FP_OPS;
	events[2]=PAPI_SP_OPS;
	events[3]=PAPI_DP_OPS;
	events[0]=PAPI_VEC_SP;
	events[4]=PAPI_VEC_DP;

	counts[0] = 0;
	counts[1] = 0;
	counts[2] = 0;
	counts[3] = 0;
	counts[4] = 0;
*/
	int retval;
	retval = PAPI_library_init(PAPI_VER_CURRENT);
	retval = PAPI_create_eventset(&EventSet);
	//PAPI_start_counters(events,2);
	start_us = PAPI_get_real_usec();

retval = PAPI_add_events(EventSet, Events, NUM_EVENTS);
	retval = PAPI_start(EventSet);
	matrix_mult_ikj ( mat_a, mat_b, mat_c, size);	
retval = PAPI_stop(EventSet, counts);

	stop_us = PAPI_get_real_usec();
//	PAPI_stop_counters(counts,2);

	long_long duration_us = stop_us - start_us;

	FILE *file;
	file = fopen(argv[2],"a");

	fprintf(file, "%lld,", duration_us );
	printf("%lld,", duration_us );	
	fprintf(file,"%lld,", counts[0]);	
	printf("%lld,", counts[0]);	
	fprintf(file,"%lld,", counts[1]);	
	printf("%lld,", counts[1]);	
	fprintf(file,"%lld,", counts[2]);	
	printf("%lld,", counts[2]);	
	fprintf(file,"%lld,", counts[3]);	
	printf("%lld\n", counts[3]);	
	fprintf(file,"%lld\n", counts[4]);	
	printf("%lld\n", counts[4]);	

	fclose(file);
	return 0;
}

