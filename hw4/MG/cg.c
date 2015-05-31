#include <stdlib.h>
#include <stdio.h>
#include "nrutil.h"
#include "mg.h"

double dotprod(int n, double **r1, double **r2) {
    double rdot = 0;
    for (int i = 1; i <= n; ++i)
        for(int j = 1; j <= n; ++j) 
            rdot += r1[i][j] * r2[i][j];
    return rdot;
}

void cg(double **xold, double **x, int n) {
    double alpha;
    double beta;
    double **r = dmatrix(1,n,1,n);
    double **p = dmatrix(1,n,1,n);
    double **Ap = dmatrix(1,n,1,n);

    //r = b - Ax
    for(int i = 2; i < n; ++i) 
        for(int j = 2; j < n; ++j) {
            double b = (1-4*C)*xold[i][j] + 
                C*(xold[i-1][j] + xold[i+1][j] + xold[i][j-1] + xold[i][j+1]);
            r[i][j] = b - (1+4*C)*x[i][j] + 
                C*(x[i-1][j] + x[i+1][j] + x[i][j-1] + x[i][j+1]);
        }
    //boundaries of r
    for(int i = 1; i <= n; ++i)
        r[i][1] = r[i][n] = r[1][i] = r[n][i] = 0;

    //p = r
    copy(p, r, n);

    double rsold, rsnew;
    rsold = dotprod(n, r, r);

    //main iterative loop of cg
    int k;
    for (k = 0; k < n; ++k) {
        //Ap = A*p
        for(int i = 2; i < n; ++i) 
            for(int j = 2; j < n; ++j) 
                Ap[i][j] = (1+4*C)*p[i][j] 
                    - C*(p[i-1][j] + p[i+1][j] + p[i][j-1] + p[i][j+1]);
        for(int i = 1; i <= n; ++i)
            Ap[i][1] = Ap[i][n] = Ap[1][i] = Ap[n][i] = 0;

        double pAp = dotprod(n, p, Ap);
        alpha = rsold / pAp;

        for(int i = 2; i < n; ++i) 
            for(int j = 2; j < n; ++j) {
                x[i][j] += alpha*p[i][j];
                r[i][j] -= alpha*Ap[i][j];
            }

        rsnew = dotprod(n, r, r);
        if (rsnew < 1e-10) break;

        beta = rsnew / rsold;
        for(int i = 2; i < n; ++i) 
            for(int j = 2; j < n; ++j) 
                p[i][j] = r[i][j] + beta*p[i][j];
        rsold = rsnew;
    }

    copy(xold, x, n); //copy solution into xold (implicit return)

    free_dmatrix(r, 1, n, 1, n);
    free_dmatrix(p, 1, n, 1, n);
    free_dmatrix(Ap, 1, n, 1, n);
}

