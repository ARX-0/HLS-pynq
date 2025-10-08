#include "loop_imperfect.hpp"

int main() {
    din_t A[N];
    dout_t B[N];

    int i, retval = 0;


    for (i = 0; i < N; ++i) {
        A[i] = i;
    }

    // Call the function
    loop_imperfect(A, B);
    //loop_perfec(A,B)
    for (i = 0; i < N; ++i) {
        cout << B[i] << endl;
    }
}
