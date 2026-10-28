#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mmio.h"

struct CSRMatrix {
    int *rowptrs;
    int *col_idx;
    double *values;
    int M;
};

struct ELLMatrix {
    int *col_idx;
    double *values;
    int max_len;
    int M;
};

void dense_mult(int M, int N, size_t nmults, double **mat, double **vec)
{
    double *tmp;
    double *in = *vec;
    double *out = malloc(M*sizeof(double));
    double sum;

    for (size_t n = 0; n < nmults ; n++) {
        for (int i = 0; i < M; i++) {
            sum = 0;
            for (int j = 0; j < N; j++) {
                    sum += mat[i][j] * in[j];
                }
            out[i] = sum;
            }
        // pointer swaps
        tmp = in;
        in = out;
        out = tmp;
        }

    *vec = in;
    free(out);

}

double **dense_convert(int M, int N, int *I, int *J, double *val, int nz)
{
    double **A = NULL;

    A = (double **)malloc(M * sizeof(*A));
    for (int r = 0; r < M; r++)
    {
        A[r] = calloc(N, sizeof(double));
    }

    for (int i = 0; i < nz; i++)
    {
        A[I[i]][J[i]] = val[i];
    }

    return A;
}

void coo_mult(int M, int nz, size_t nmults, int *I, int *J, double *val, double **vec)
{
     // TODO
    double *tmp;
    double *in = *vec;
    double *out = malloc(M*sizeof(double));
    
    for (size_t n = 0; n < nmults ; n++) {
        for (int i = 0; i < M; i++) {
            out[i] = 0;
        }
        for (int z = 0; z < nz; z++) {
            out[I[z]] += val[z] * in[J[z]];
        }
        tmp = in;
        in = out;
        out = tmp;
    }
    *vec = in;
    free(out);
}

struct CSRMatrix csr_convert(int M, int *I, int *J, double *vals, int nz)
{
    struct CSRMatrix A;
    int *next;
    int r, dest;
    int *counts = calloc(M, sizeof(int));

    A.M = M;
    A.rowptrs = malloc((M+1)*sizeof(int));
    A.values = malloc(nz*sizeof(double));
    A.col_idx = malloc(nz*sizeof(int));

    for (int i=0; i<nz; i++) {
        counts[I[i]]++;
    }
    A.rowptrs[0] = 0;
    for (int i = 0; i < M; i++) {
        A.rowptrs[i+1] = A.rowptrs[i] + counts[i];
    }
    next = counts;
    // each element of next holds the starting pointers
    for (int i = 0; i < M; i++) {
        next[i] = A.rowptrs[i];
    }
    for (int k = 0; k < nz; k++) {
        r = I[k];
        dest = next[r];
        A.col_idx[dest] = J[k];
        A.values[dest] = vals[k];
        // now point to the next slot for row r
        next[r]++;
    }
    free(counts);
    return A;
}

void csr_mult(struct CSRMatrix csr, size_t nmults, double **vec)
{
    double *tmp;
    double *in = *vec;
    double *out = malloc(csr.M*sizeof(double));
    double sum; 

    for (size_t n = 0; n < nmults; n++) {
        for (int i = 0; i < csr.M; i++) {
            sum = 0;
            for (int c = csr.rowptrs[i]; c < csr.rowptrs[i+1]; c++) {
                sum += csr.values[c] * in[csr.col_idx[c]];
            }            
            out[i] = sum;
        }
        // pointer swaps
        tmp = in;
        in = out;
        out = tmp;
    }

    *vec = in;
    free(out);
}

void csr_free(struct CSRMatrix csr)
{
    free(csr.rowptrs);
    free(csr.col_idx);
    free(csr.values);
}

struct ELLMatrix ell_convert(struct CSRMatrix csr) {
    int max_len, len;
    struct ELLMatrix ell;

    ell.M = csr.M;

    max_len = 0;
    for (int i = 0; i < ell.M; i++) {
        len = csr.rowptrs[i+1] - csr.rowptrs[i];
        if (len > max_len) max_len = len;
    }

    ell.max_len = max_len;
    ell.col_idx = (int *)calloc(ell.M*ell.max_len, sizeof(int));
    ell.values = (double *)calloc(ell.M*ell.max_len, sizeof(double));

    for (int i = 0; i < ell.M; i++) {
        len = csr.rowptrs[i+1] - csr.rowptrs[i];
        for (int c = 0; c < len; c++) {
            // entry c of row i: CSR slot rowptrs[i]+c -> column-major ELL slot c*M+i
            ell.col_idx[c*ell.M + i] = csr.col_idx[csr.rowptrs[i] + c];
            ell.values[c*ell.M + i] = csr.values[csr.rowptrs[i] + c];
        }
    }
    return ell;
}

void ell_mult(struct ELLMatrix ell, size_t nmults, double **vec)
{
    double *tmp;
    double *in = *vec;
    double *out = malloc(ell.M*sizeof(double));
    double sum; 

    for (size_t n = 0; n < nmults; n++) {
        for (int i = 0; i < ell.M; i++) {
            sum = 0;
            for (int c = 0; c < ell.max_len; c++) {
                sum += ell.values[c*ell.M + i] * in[ell.col_idx[c*ell.M + i]];
            }            
            out[i] = sum;
        }
        // pointer swaps
        tmp = in;
        in = out;
        out = tmp;
    }

    *vec = in;
    free(out);
}

void ell_free(struct ELLMatrix ell) {
    free(ell.col_idx);
    free(ell.values);
} 

int main(int argc, char *argv[])
{
    if (argc != 6) {
        printf("Not enough arguments. Usage is:\n");
        printf("SpMV.exe <spfmt> <nmults> <mmfile> <vecfilein> <vecfileout>\n");
        return 1;
    }

    char *spfmt = argv[1];
    size_t nmults = atoi(argv[2]);
    char *mmfile = argv[3];
    char *vecfilein = argv[4];
    char *vecfileout = argv[5];
    MM_typecode matcode;
    FILE *f, *fvin, *fvout;
    int M, N, nz;
    int i, k, *I, *J;
    double *val, *vec;
    double **A = NULL;

    f = fopen(mmfile, "r");
    mm_read_banner(f, &matcode);
    mm_read_mtx_crd_size(f, &M, &N, &nz);
    if (mm_is_symmetric(matcode)) {
        I = (int *)malloc(2*nz * sizeof(int));
        J = (int *)malloc(2*nz * sizeof(int));
        val = (double *)malloc(2*nz * sizeof(double));
        k = 0;
        for (i = 0; i < nz; i++) {
            fscanf(f, "%d %d %lg\n", &I[k], &J[k], &val[k]);
            // go from 1 to 0 based indexing
            I[k]--;
            J[k]--;
            k++;
            // the entry just stored is at k-1; mirror unless it's on the diagonal
            if (I[k-1] != J[k-1]) {
                I[k] = J[k-1];
                J[k] = I[k-1];
                val[k] = val[k-1];
                k++;
            }
        }
        nz = k;
    } else {
        I = (int *)malloc(nz * sizeof(int));
        J = (int *)malloc(nz * sizeof(int));
        val = (double *)malloc(nz * sizeof(double));
        for (i = 0; i < nz; i++) {
            fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
            // go from 1 to 0 based indexing
            I[i]--;
            J[i]--;
        }
    }

    vec = (double *)malloc(M * sizeof(double));


    if (f != stdin) fclose(f);

    fvin = fopen(vecfilein, "r");
    for (i = 0; i < M ; i++)
    {
        fscanf(fvin, "%lg\n", &vec[i]);
    }
    if (fvin != stdin) fclose(fvin);

    clock_t t0 = 0, t1 = 0;
    if (!strcmp(spfmt, "DEN")) {
        A = dense_convert(M, N, I, J, val, nz);
        t0 = clock();
        dense_mult(M, N, nmults, A, &vec);
        t1 = clock();
        for (int r=0; r<M ; r++) {
            free(A[r]);
        }

        free(A);
    } else if (!strcmp(spfmt, "COO")) {
        t0 = clock();
        coo_mult(M, nz, nmults, I, J, val, &vec);
        t1 = clock();
    } else if (!strcmp(spfmt, "CSR")) {
        struct CSRMatrix csr = csr_convert(M, I, J, val, nz);
        t0 = clock();
        csr_mult(csr, nmults, &vec);
        t1 = clock();
        csr_free(csr);
    } else if (!strcmp(spfmt, "ELL")) {
        struct CSRMatrix csr = csr_convert(M, I, J, val, nz);
        struct ELLMatrix ell = ell_convert(csr);
        csr_free(csr);
        t0 = clock();
        ell_mult(ell, nmults, &vec);
        t1 = clock();
        ell_free(ell);
    }

    double total = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("%s %s nmults=%zu total=%.6e s avg_per_spmv=%.6e s\n",
           spfmt, mmfile, nmults, total, total / nmults);

    fvout = fopen(vecfileout, "w"); 
    for (i = 0; i< M; i++) {
        fprintf(fvout, "%.16lg\n", vec[i]);
    }
    if (fvout != stdout) fclose(fvout);

    free(I);
    free(J);
    free(val);
    free(vec);


    return 0;
}
