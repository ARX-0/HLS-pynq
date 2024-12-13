/*
	Filename: fir.cpp
		FIR lab written for WES/CSE237C class at UCSD.

	INPUT:
		x: signal (chirp)

	OUTPUT:
		y: filtered output
*/

#include "fir.h"
#define N 11
#include "ap_int.h"

typedef int coef_t;
typedef int data_t;
typedef int acc_t;

void fir (
  data_t *y,
  data_t x
  )
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0, 53};  //11 register values
	// Write your code here
	static data_t s_r[N];
#pragma HLS ARRAY_PARTITION variable=s_r type=complete
	acc_t acc;
	int i;
for(int n =0;n < SAMPLES ; n++){
#pragma HLS PIPELINE II=1
	acc = 0;
	// Shift Acc Loop
	Shift_Loop: for (int i = N - 1; i >= 0; i--) {
#pragma HLS UNROLL  // Unroll the loop partially by a factor of 2 for parallelism

		/* if (i == 0) {
			acc += x * c[0];//2
			s_r[0] = x; //1
		} else {

			s_r[i] = s_r[i - 1]; //shift register //1
			acc += s_r[i] * c[i];}//2
		*/

		// optimize this coding style code refactoring ie separating the 0 state
		s_r[i] = (i==0) ? x : s_r[i - 1]; //1
		data_t x_temp = (i==0) ? x : s_r[i]; //2
		acc += x_temp * c[i];
	}
	*y = acc; // output being the sum of the accumulator
}
}
