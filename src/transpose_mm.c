#include <stdio.h>
#include <stdlib.h>
#include "papi.h"

#define MATRIX_SIZE 4
#define NUM_EVENTS 10

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


void fprint_matrix (FILE* fp, float **b){
	for (unsigned i=0;i<MATRIX_SIZE;i++){
		for(unsigned j=0;j<MATRIX_SIZE;j++){
			fprintf(fp,"%f,",b[i][j]);
		}
		fprintf(fp,"\n");
	}
}

void print_matrix ( float **b, int N){
	for (unsigned i=0;i<MATRIX_SIZE;i++){
		for(unsigned j=0;j<MATRIX_SIZE;j++){
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

	//Fill Matrices
	fillMatrices_232(mat_a, mat_b, mat_c, MATRIX_SIZE );
	clearCache();

	/* PAPI VARIABLES */
	int events[NUM_EVENTS];
	long long counts[NUM_EVENTS];

	/*MAIN MEMORY ACESSES PER INSTRUCTION*/
	events[0]=PAPI_L3_TCM;
	events[1]=PAPI_FP_INS;

	/* NUMBER OF BYTES TRANSFERED TO/FROM RAM */
	events[2]=PAPI_L3_TCR;
	events[3]=PAPI_L3_TCW;

	/* FLOATING POINT OPERATIONS */
	events[4]=PAPI_FP_OPS;

	/* MISS RATE L1 L2 L3 */
	events[5]=PAPI_L1_TCR;
	events[6]=PAPI_L2_TCR;
	events[7]=PAPI_L1_TCM;
	events[8]=PAPI_L2_TCM;
	events[9]=PAPI_L3_TCM;

	counts[0] = 0;
	counts[1] = 0;
	counts[2] = 0;
	counts[3] = 0;
	counts[4] = 0;
	counts[5] = 0;
	counts[6] = 0;
	counts[7] = 0;
	counts[8] = 0;
	counts[9] = 0;

	PAPI_library_init(PAPI_VER_CURRENT);
	PAPI_start_counters(events,2);

	start_us = PAPI_get_real_usec();
	printf("\nXXXXXXXXXXXXXXXXXXXXX\n\n");

	print_matrix(mat_a , MATRIX_SIZE);
	printf("\nccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n\n");
	print_matrix(mat_b , MATRIX_SIZE);
	printf("\nccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n\n");
	print_matrix(mat_c , MATRIX_SIZE);

	printf("\nccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n\n");
	FILE *file_a;
	file_a = fopen("matrix_a.csv","w");
	fprint_matrix(file_a, mat_a);
	fclose(file_a);

	FILE *file_b;
	file_b = fopen("matrix_b.csv","w");
	fprint_matrix(file_b, mat_b);
	fclose(file_b);


	matrix_mult_ijk ( mat_a, mat_b, mat_c, MATRIX_SIZE);	
	print_matrix(mat_c , MATRIX_SIZE);
	FILE *file_c_ijk_ab;
	file_c_ijk_ab = fopen("matrix_ijk_ab.csv","w");
	fprint_matrix(file_c_ijk_ab, mat_c);
	fclose(file_c_ijk_ab);

	clearMatrix(mat_c, MATRIX_SIZE);

	printf("\nXXXXXXXXXXXXXXXXXXXXX\n\n");
	matrix_mult_ijk ( mat_b, mat_a, mat_c, MATRIX_SIZE);	
	print_matrix(mat_c , MATRIX_SIZE);
	FILE *file_c_ijk_ba;
	file_c_ijk_ba = fopen("matrix_ijk_ba.csv","w");
	fprint_matrix(file_c_ijk_ba, mat_c);
	fclose(file_c_ijk_ba);

	clearMatrix(mat_c, MATRIX_SIZE);

	printf("\nXXXXXXXXXXXXXXXXXXXXX\n\n");
	matrix_mult_ikj ( mat_a, mat_b, mat_c, MATRIX_SIZE);	
	print_matrix(mat_c , MATRIX_SIZE);
	clearMatrix(mat_c, MATRIX_SIZE);

	printf("\nXXXXXXXXXXXXXXXXXXXXX\n\n");
	matrix_mult_ikj ( mat_b, mat_a, mat_c, MATRIX_SIZE);	
	print_matrix(mat_c , MATRIX_SIZE);
	clearMatrix(mat_c, MATRIX_SIZE);

	printf("\nXXXXXXXXXXXXXXXXXXXXX\n\n");
	matrix_mult_ijk_transposed ( mat_a, mat_b, mat_c, MATRIX_SIZE);	
	print_matrix(mat_c , MATRIX_SIZE);

	FILE *file_c_ijk_trans_ab;
	file_c_ijk_trans_ab = fopen("matrix_ijk_trans_ab.csv","w");
	fprint_matrix(file_c_ijk_trans_ab, mat_c);
	fclose(file_c_ijk_trans_ab);

	clearMatrix(mat_c, MATRIX_SIZE);
	printf("\nXXXXXXXXXXXXXXXXXXXXX\n\n");
	matrix_mult_ijk_transposed ( mat_b, mat_a, mat_c, MATRIX_SIZE);	
	print_matrix(mat_c , MATRIX_SIZE);
	FILE *file_c_ijk_trans_ba;
	file_c_ijk_trans_ba = fopen("matrix_ijk_trans_ba.csv","w");
	fprint_matrix(file_c_ijk_trans_ba, mat_c);
	fclose(file_c_ijk_trans_ba);


	clearMatrix(mat_c, MATRIX_SIZE);


	PAPI_stop_counters(counts,2);


	stop_us = PAPI_get_real_usec();

	long_long duration_us = stop_us - start_us;

	printf("%lld\n", duration_us );
	for(int i = 0; i<NUM_EVENTS; i++){
		//		fprintf(file,"%lld,", counts[i]);	
		printf("%lld\t", counts[i]);	
	}

	return 0;
}

