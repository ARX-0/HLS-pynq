#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "top.hpp"
#include "hls_stream.h"

// Utility to print a matrix
void print_matrix(const std::vector<my32bitinteger> &M, int size, const std::string &name) {
    std::cout << name << " = \n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << M[i * size + j] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Golden software model
void golden_model(const std::vector<my32bitinteger> &A,
                  const std::vector<my32bitinteger> &B,
                  std::vector<my32bitinteger> &C_add,
                  std::vector<my32bitinteger> &C_sub,
                  std::vector<my32bitinteger> &C_mul,
                  int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int idx = i * size + j;
            my32bitinteger sum = 0;
            for (int k = 0; k < size; k++) {
                sum += A[i * size + k] * B[k * size + j];
            }
            C_add[idx] = A[idx] + B[idx];
            C_sub[idx] = A[idx] - B[idx];
            C_mul[idx] = sum;
        }
    }
}

int main() {
    const int size = 4;
    const int total_elems = size * size;

    // Input matrices
    std::vector<my32bitinteger> A(total_elems), B(total_elems);

    srand(time(0));
    for (int i = 0; i < total_elems; i++) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    print_matrix(A, size, "Matrix A");
    print_matrix(B, size, "Matrix B");

    // Streams
    hls::stream<my32bitinteger> A_stream, B_stream;
    hls::stream<my32bitinteger> C_add_stream, C_sub_stream, C_mul_stream;

    // Push inputs into streams
    for (int i = 0; i < total_elems; i++) {
        A_stream.write(A[i]);
        B_stream.write(B[i]);
    }

    // Run hardware top
    top(A_stream, B_stream,
        C_add_stream, C_sub_stream, C_mul_stream,
        size);

    // Collect hardware results
    std::vector<my32bitinteger> C_add_hw(total_elems), C_sub_hw(total_elems), C_mul_hw(total_elems);
    for (int i = 0; i < total_elems; i++) {
        C_add_hw[i] = C_add_stream.read();
        C_sub_hw[i] = C_sub_stream.read();
        C_mul_hw[i] = C_mul_stream.read();
    }

    // Run golden software
    std::vector<my32bitinteger> C_add_sw(total_elems), C_sub_sw(total_elems), C_mul_sw(total_elems);
    golden_model(A, B, C_add_sw, C_sub_sw, C_mul_sw, size);

    // Print results
    print_matrix(C_add_hw, size, "HW C_add");
    print_matrix(C_add_sw, size, "SW C_add");
    print_matrix(C_sub_hw, size, "HW C_sub");
    print_matrix(C_sub_sw, size, "SW C_sub");
    print_matrix(C_mul_hw, size, "HW C_mul");
    print_matrix(C_mul_sw, size, "SW C_mul");

    // Compare results
    int errors = 0;
    for (int i = 0; i < total_elems; i++) {
        if (C_add_hw[i] != C_add_sw[i]) {
            std::cout << "Mismatch in ADD at index " << i
                      << ": HW=" << C_add_hw[i] << " SW=" << C_add_sw[i] << "\n";
            errors++;
        }
        if (C_sub_hw[i] != C_sub_sw[i]) {
            std::cout << "Mismatch in SUB at index " << i
                      << ": HW=" << C_sub_hw[i] << " SW=" << C_sub_sw[i] << "\n";
            errors++;
        }
        if (C_mul_hw[i] != C_mul_sw[i]) {
            std::cout << "Mismatch in MUL at index " << i
                      << ": HW=" << C_mul_hw[i] << " SW=" << C_mul_sw[i] << "\n";
            errors++;
        }
    }

    if (errors == 0)
        std::cout << "\nTest Passed! HW and SW results match.\n";
    else
        std::cout << "\n Test Failed with " << errors << " mismatches.\n";

    return errors;
}
