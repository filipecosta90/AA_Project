#include <stdio.h>

int main (int argc, char *argv[]){
	FILE *file;
	file = fopen(argv[1],"w");
	fprintf(file,"Execution Time,PAPI_FP_OPS,PAPI_TOT_CYC,PAPI_TOT_INS,PAPI_LD_INS,PAPI_L1_TCM,PAPI_L2_TCM,PAPI_L3_TCM\n");
	printf("Execution Time\tPAPI_FP_OPS\tPAPI_TOT_CYC\tPAPI_TOT_INS\tPAPI_LD_INS\tPAPI_L1_TCM\tPAPI_L2_TCM\tPAPI_L3_TCM\n");
	fclose(file);
	return 0;
}

