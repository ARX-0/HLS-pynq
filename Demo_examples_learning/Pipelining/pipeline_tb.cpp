#include "pipeline.hpp"

int main() {

    int i, retval = 0;
    const char N = 10;
    char a, b, c;

    for (i = 0; i < N; ++i) {
        // Call the function
        top(i, i, i, &a, &b, &c);
        cout << (int)a << " " << (int)b << " " << (int)c << endl;
    }
}
