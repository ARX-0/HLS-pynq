#include "loop_imperfect.hpp"

int loop_pipeline(dint_t A[N]) {
 int i,j;
 static dout_t acc;

 LOOP_I: for(i=0;i<=N;i++){
	 LOOP_J: for(j=0; j<=N;j++){
		 acc += A[j] * i;
	 }
 }
return acc;
}
