#include "apint.h"
#include <iostream>

typedef int integer;
typedef int N,M,P;

void matrixmul(integer A[N][M],integer B[M][P], integer AB[N][P]){
row:for(integer i = 0 ; i<N ; ++i){
    col:for(integer j=0 ; j<P ; ++j){
        integer ABij = 0;
        /* here we initailize (AB)i,j */
    product:for(integer k=0; k<M ; ++k){
     ABij += A[i][k] * B[k][j] ;//or ABij = ABij + A[i][k] * B[k][j] ;
    }
    AB[i][j] = ABij;
    }
  }
}