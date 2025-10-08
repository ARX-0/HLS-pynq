#ifndef _LOOP_PIPELINE_H_
#define _LOOP_PIPELINE_H_

#include <fstream>
#include <iostream>

using namespace std;

#include "ap_int.h"
#define N 5
#define NUM_TRANS 5

typedef int din_t;
typedef int dout_t;

int loop_pipeline(din_t A[N]);

#endif
