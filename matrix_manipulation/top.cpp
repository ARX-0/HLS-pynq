#include "top.hpp"

extern "C" {
void top(hls::stream<my32bitinteger> &A_stream,
         hls::stream<my32bitinteger> &B_stream,
         hls::stream<my32bitinteger> &C_add_stream,
         hls::stream<my32bitinteger> &C_sub_stream,
         hls::stream<my32bitinteger> &C_mul_stream,
         int size) {
#pragma HLS PIPELINE
    // -----------------------------
    // Step 1: Read input streams into local arrays
    // -----------------------------
    my32bitinteger localA[N*N]; // assuming max size <= 8x8 (adjust as needed)
    my32bitinteger localB[N*N];
#pragma HLS ARRAY_PARTITION variable=localA complete dim=0
#pragma HLS ARRAY_PARTITION variable=localB complete dim=0

    for (int i = 0; i < N*N; i++) {
#pragma HLS UNROLL
        localA[i] = A_stream.read();
        localB[i] = B_stream.read();
    }

    // -----------------------------
    // Step 2: Create qbMatrix2 objects
    // -----------------------------
    qbMatrix2<my32bitinteger> matA(size, size, localA);
    qbMatrix2<my32bitinteger> matB(size, size, localB);

    qbMatrix2<my32bitinteger> matC_add = matA + matB;
    qbMatrix2<my32bitinteger> matC_sub = matA - matB;
    qbMatrix2<my32bitinteger> matC_mul = matA * matB;

    // -----------------------------
    // Step 3: Write results back into output streams
    // -----------------------------
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < size; j++) {
#pragma HLS UNROLL
            C_add_stream.write(matC_add.GetElement(i,j));
            C_sub_stream.write(matC_sub.GetElement(i,j));
            C_mul_stream.write(matC_mul.GetElement(i,j));
        }
    }
}
}
