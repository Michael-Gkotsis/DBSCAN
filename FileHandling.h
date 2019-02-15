#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// n = rows
// dim = columns


int getColumnsTab(FILE*);
int getColumnsComma(FILE*);
int getColumnsSpace(FILE*);
int getRows(FILE*);
double **getDataTab(FILE*,int,int,double**);
double **getDataSpace(FILE*,int,int,double**);
double **getDataComma(FILE*,int,int,double**);
void writeFile(FILE*,int,int,double **);
double **transformPositive(int,int,double **);


int getColumnsTab(FILE* Dataset)
{
    int dim = 0;         // Dimensions of Elements
  char c;              // Temporary character for file analysis
  do
  {
      fscanf(Dataset, "%c", &c);
      if (c == '\t')
          dim++;
  } while (c != '\n');
     dim++;
  rewind(Dataset);  // File reset

  return dim;
}

int getColumnsComma(FILE* Dataset)
{
    int dim = 0;         // Dimensions of Elements
  char c;              // Temporary character for file analysis
  do
  {
      fscanf(Dataset, "%c", &c);
      if (c == ',')
          dim++;
  } while (c != '\n');
     dim++;
  rewind(Dataset);  // File reset

  return dim;
}
int getColumnsSpace(FILE* Dataset)
{
    int dim = 0;         // Dimensions of Elements
  char c;              // Temporary character for file analysis
  do
  {
      fscanf(Dataset, "%c", &c);
      if (c == ' ')
          dim++;
  } while (c != ' ');
     dim++;
  rewind(Dataset);  // File reset

  return dim;
}



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


double **getDataTab(FILE* Dataset,int n,int dim,double **X)
{
  int i;
  int d;
  while(!feof(Dataset))
   for (i = 0;  i < n; i++)
   {
       for (d = 0; d < dim; d++)
       {
           fscanf(Dataset, "%lf\t", &X[i][d]);
       }
   }

   return X;

}

double **getDataSpace(FILE* Dataset,int n,int dim,double **X)
{
  int i;
  int d;
  while(!feof(Dataset))
   for (i = 0;  i < n; i++)
   {
       for (d = 0; d < dim; d++)
       {
           fscanf(Dataset, "%lf ", &X[i][d]);
       }
   }

   return X;

}

double **getDataComma(FILE* Dataset,int n,int dim,double **X)
{
  int i;
  int d;
  while(!feof(Dataset))
   for (i = 0;  i < n; i++)
   {
       for (d = 0; d < dim; d++)
       {
           fscanf(Dataset, "%lf,", &X[i][d]);
       }
   }

   return X;

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
