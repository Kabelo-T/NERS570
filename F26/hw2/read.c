#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"

int main(int argc, char *argv[])
{
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;
    int i, *I, *J;
    double *val;
    double ** A = NULL;

    f = fopen(argv[1], "r");
    mm_read_banner(f, &matcode);
    printf("code: %s \n", mm_typecode_to_str(matcode));
    mm_read_mtx_crd_size(f, &M, &N, &nz);
    printf("M = %d ; N = %d ; nz = %d \n", M, N, nz);

    A = (double **) malloc(M*sizeof(*A));
    for (int r = 0; r<M; r++)
    {
        A[r] = calloc(N, sizeof(double));
    }

    I = (int *) malloc(nz*sizeof(int));
    J = (int *) malloc(nz*sizeof(int));
    val = (double *) malloc(nz*sizeof(double));

    for (i=0; i<nz ; i++)
    {
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        // go from 1 to 0 based indexing
        I[i]--;
        J[i]--;
        A[I[i]][J[i]] = val[i];
    }

    printf("%lg\n", A[M-1][N-1]);    


    for (int r=0; r<M ; r++)
    {
        free(A[r]);
    }
    free(A);
    free(I);
    free(J);
    free(val);
    if (f !=stdin) fclose(f);
    return 0;
}
