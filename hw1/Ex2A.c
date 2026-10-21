#include <stdio.h>
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


int main(int argc, char *argv[])
{
    size_t N = atoi(argv[1]);
    if (N!=2 && N!=4 && N!=8 && N!=16) {
        printf("ERROR: N = %zu \nMust be one of [2, 4, 8, 16]\n", N);
        return 1;
    }
    int end = N*N;
    int arr[end];
    z_order(arr, N, 0, 0, N, 1);
    int len = 0;
    int tmp = end;
    do {
        tmp /=10;
        len ++;
    } while (tmp > 0);
    printf("A=[");
    for (int i=0; i<end; i++){
        if (i % N != 0) printf(" ");
        printf("%*d", len, arr[i]);
        if (arr[i] == N*N) {
            printf("]\n");
        }
        else if ((i+1) % N == 0) {
            printf("\n   ");
        }
        }
    return 0;
}
