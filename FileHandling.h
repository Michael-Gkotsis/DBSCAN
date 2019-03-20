#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// n = rows
// dim = columns



int getColumns(FILE*);
int getRows(FILE*);

float *getData(FILE*,int ,int ,float *);
void writeFile(FILE*,int,int,double **);
double **transformPositive(int,int,double **);
void qS(double *,int ,int);
int partition(double *,int ,int);
void qSort(float *,int);





int getRows(FILE* Dataset)
{
  char c;              // Temporary character for file analysis
    int n = 0;           // n Elements
  do
  {
      fscanf(Dataset, "%c", &c);
      if (c == '\n')
          n++;
  } while (!feof(Dataset));

  rewind(Dataset); // File reset



return n;
}



void writeFile(FILE* Dataset,int n, int dim, double **X)
{

int i;
int d;

  for(i = 0; i < n; i++){
        for(d = 0; d < dim; d++){
            fprintf(Dataset, "%lf",X[i][d]);
            if(d < dim - 1)
            {
              fprintf(Dataset,", ");
            }
          }
          fprintf(Dataset, "\n" );
          }
}

double **transformPositive(int n,int dim, double**X)
{
  int i;
  int d;
  for(i = 0; i < n; i++)
     for(d = 0; d < dim; d++)
     X[i][d] = fabs(X[i][d]);

return X;


}


void qS(double *reachabilityDistance,int low, int high)
{
  int pi;
  if(low < high)
  {
    pi = partition(reachabilityDistance,low,high);

    qS(reachabilityDistance,low, pi - 1);
    qS(reachabilityDistance,pi + 1, high);
  }
}

int partition(double *reachabilityDistance,int low,int high)
{
  double pivot;
 int i;
 int j;
  pivot = reachabilityDistance[high];

  i = (low - 1);

  for(j = low; j <= high - 1; j++)
  {
    if(reachabilityDistance[j] <= pivot)
    {
      i++;

      double temp = reachabilityDistance[i];
      reachabilityDistance[i] = reachabilityDistance[j];
      reachabilityDistance[j] = temp;
    }
  }
  double temp = reachabilityDistance[i + 1];
  reachabilityDistance[i + 1] = reachabilityDistance[high];
  reachabilityDistance[high] = temp;

  return(i + 1);

}





int getColumns(FILE* Dataset)
{
  int flag = 0;
  int flagPrev = 0;
  int counter = 0;
    int dim = 0;         // Dimensions of Elements
  char c;              // Temporary character for file analysis

      while (c != '\n'){
      fscanf(Dataset, "%c", &c);
      if((c >= '0' && c <= '9') ||( c == '-' || c == '+' ) || (c == '.' ))
          {
            flag = 1;
            counter++;
          }else
          {
            flag = 0;
            counter = 0;
          }
          if(counter == 0 && flagPrev != flag)
          {
            dim++;
          }
          flagPrev = flag;
        }



  rewind(Dataset);  // File reset

  return dim;
}
float *getData(FILE* Dataset,int n,int dim,float *X)
{
  int i;
  int d;
  while(!feof(Dataset))
   for (i = n; i--;)
   {
       for (d = dim; d--;)
       {
           fscanf(Dataset, "%f", &X[i*dim + d]);
       }
   }

   return X;


}



void qSort(float *arr, int elements) {

  #define  MAX_LEVELS  300

  float  piv, beg[MAX_LEVELS], end[MAX_LEVELS],  swap ;
  int i=0, L, R;
  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L];
      while (L<R) {
        while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
        while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
      arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L;
      if (end[i]-beg[i]>end[i-1]-beg[i-1]) {
        swap=beg[i]; beg[i]=beg[i-1]; beg[i-1]=swap;
        swap=end[i]; end[i]=end[i-1]; end[i-1]=swap; }}
    else {
      i--; }}}
