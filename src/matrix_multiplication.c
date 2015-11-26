#include <stdio.h>
#include <stdlib.h>

void matrix_multiplication(float **a, float **b, float **c, int N){
	for(unsigned i=0;i<N;i++){
		for(unsigned j=0;j<N;i++){
			for(unsigned k=0;k<N;k++){
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

int main (int argc, char *argv[]){
	return 1;
}

