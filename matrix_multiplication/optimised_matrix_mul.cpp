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
     ABij += A[i][k] * B[k][j] ;//MAC (*,+) operation to be pipelined
     //1 element in array A and 1 element from array B as supply (each element)
    }
    AB[i][j] = ABij;
    }
  }
}
// pipelining look i gets you :( highest resource util and the :) highest throughput
// pipelining look i gets you :) lowest resource util and the :( lowest throughput 
//so we do j loop the middle ground