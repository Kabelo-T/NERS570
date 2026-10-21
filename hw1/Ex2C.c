#include <stdio.h>
#include <stdlib.h>

int interleave_bits(int x) {
    // in case N=16 need to split into high 4 bits and low 4 bits
    x = (x | (x << 4)) & 0x0F0F;
    // split each 2-bit group into its own 4-bit group 0011 --> 0000 0011
    x = (x | (x << 2)) & 0x3333;
    // add single 0s between each bit
    x = (x | (x << 1)) & 0x5555;
    // final result is bit i of x is mapped to bit 2i
    return x;
}

int z_index(int row, int col) {
    // mix the even column bits with the odd row bits
    return interleave_bits(col) | (interleave_bits(row) << 1);
}

void z_fill_bitwise(int *arr, int N, int start_index) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            arr[row * N + col] = z_index(row, col) + start_index;
        }
    }
}


int main(int argc, char *argv[]){
    size_t N = atoi(argv[1]);
    if (N!=2 && N!=4 && N!=8 && N!=16) {
        printf("ERROR: N = %zu \nMust be one of [2, 4, 8, 16]\n", N);
        return 1;
    }
    int end = N*N;
    int arr[end];
    z_fill_bitwise(arr, N, 1);    
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

