#include <stdio.h>
#include <stdlib.h>

void z_order_f(int *arr, int N, int rstart, int cstart, int size, int val);

int main(int argc, char *argv[])
{
    size_t N = atoi(argv[1]);
    if (N!=2 && N!=4 && N!=8 && N!=16) {
        printf("ERROR: N = %zu \nMust be one of [2, 4, 8, 16]\n", N);
        return 1;
    }
    int end = N*N;
    int arr[end];
    z_order_f(arr, N, 0, 0, N, 1);
    int len = 0;
    int tmp = end;
    do {
        tmp /=10;
        len ++;
    } while (tmp > 0);
    printf("A=[");
    for (int i=0; i<end; i++){
        if (i % N != 0) printf(" ");
        printf("%*d", len, arr[(i%N)*N + i/N]);
        if (arr[i] == N*N) {
            printf("]\n");
        }
        else if ((i+1) % N == 0) {
            printf("\n   ");
        }
        }
    return 0;
}
