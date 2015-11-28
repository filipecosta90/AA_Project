void matrix_mult_ikj (float **a, float **b, float **c, int N){
  for(unsigned i=0;i<N;i++){
    for(unsigned k=0;k<N;k++){
      for(unsigned j=0;j<N;j++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}
