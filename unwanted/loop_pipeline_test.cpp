#include "loop_pipeline.hpp"

int main() {
    din_t A[N];
    dout_t accum;

    int i, j, retval = 0;

    // Create input data
    for (i = 0; i < N; ++i) {
        A[i] = i;
    }


    // Call the function
    for (j = 0; j < NUM_TRANS; ++j) {
        accum = loop_pipeline(A);
        cout << accum << endl;
    }
}
