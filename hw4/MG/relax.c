#include "mg.h"

void relax(double **u, double **xold, int n)
   /*
      Red-black Gauss-Seidel relaxation for model problem. Updates the current value of the solution
      u[1..n][1..n], using the right-hand side function rhs[1..n][1..n].

      DO NOT USE SOR HERE!
      */
{
   int i,ipass,isw,j,jsw=1;
   /* Red and black sweeps.*/
   /* jsw and isw toggle between 1 and 2 and
      determine starting row in each column
      for given pass 
      */
   for (ipass=1;ipass<=2;ipass++,jsw=3-jsw) { 
      isw=jsw;
      for (j=2;j<n;j++,isw=3-isw)
         /*Gauss-Seidel formula.*/
         for (i=isw+1;i<n;i+=2) {
            double b = (1-4*B)*xold[i][j] + B*(xold[i-1][j] + xold[i+1][j] + xold[i][j-1] + xold[i][j+1]);
            u[i][j] = (b + C*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]))/(1+4*C);
         }
   }
}
