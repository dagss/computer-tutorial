#include <stdio.h>
#include <stdint.h>

/* A very stupid way to compute fibonacci, but allows showing off the stack */
int fibonacci(int n) {
    int result;
    printf("> Entering fibonacci(%3d), &n == %ld\n", n, (size_t)&n);
    if (n <= 2) {
        result = 1;
    } else {
        result = fibonacci(n - 1) + fibonacci(n - 2);
    }
    printf("> Exiting fibonacci(%3d) with result %d\n", n, result);
    return result;
}

int main() {
    printf("%d\n", fibonacci(10));
    return 0;
}
