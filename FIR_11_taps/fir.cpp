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
	acc_t acc;
	int i;

	acc = 0;
	//Shift Acc Loop
	for (int i = N - 1; i >= 0; i--) {
		if (i == 0) {
			acc += x * c[0];
			s_r[0] = x;
		} else {
			s_r[i] = s_r[i - 1]; //shift register
			acc += s_r[i] * c[i];
		}
	}
	*y = acc; // output being the sum of the accumulator
}
