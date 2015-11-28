void matrix_mult_ijk (float **a, float **b, float **c, int N){
  for(unsigned i=0;i<N;i++){
    for(unsigned j=0;j<N;i++){
      for(unsigned k=0;k<N;k++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}
