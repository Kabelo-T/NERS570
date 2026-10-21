#include <stdlib.h>

void z_order(int *arr, int N, int rstart, int cstart, int size, int val){
    // base case, we are in a 2x2 cell
    if (size == 2) {
        size_t base = rstart*N + cstart;
        arr[base] = val;        // tl
        arr[base+1] = val + 1;  // tr
        arr[base+N] = val+2;    // bl
        arr[base+N+1] = val+3;          // br
        return;
    }

    // recursive step, there are more quadrants in this sizexsize cell

    int half = size/2;
    int quadrants = (size*size)/4;

    z_order(arr, N, rstart, cstart, half, val);                     // tl
    z_order(arr, N, rstart, cstart+half, half, val+quadrants);      // tr
    z_order(arr, N, rstart+half, cstart, half, val+2*quadrants);    // bl
    z_order(arr, N, rstart+half, cstart+half, half, val+3*quadrants); // br
}
