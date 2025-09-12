#ifndef TOP_H
#define TOP_H

#include "matrix_manipulation.hpp"
#include <ap_int.h>
#include <hls_stream.h>

#define N 4
//#define size 4
typedef ap_int<32> my32bitinteger;  // 32-bit integer type for HLS streams

// Stream-based top function
extern "C" {
void top(hls::stream<my32bitinteger> &A_stream,
         hls::stream<my32bitinteger> &B_stream,
         hls::stream<my32bitinteger> &C_add_stream,
         hls::stream<my32bitinteger> &C_sub_stream,
         hls::stream<my32bitinteger> &C_mul_stream,
         int size);
}

#endif
