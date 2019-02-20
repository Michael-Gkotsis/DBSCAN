#include <stdio.h>
#include <stdlib.h>
#include "FileHandling.h"
#include <math.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{

   int dim = 0;         // Dimensions of Elements
   int n = 0;           // n Elements
   int i,j,d;           // i counter for n, j counter for Neighborhood Elements, d counter for dimensions
   double minDist;      // Minimum Distance for a point to be assigned to clusters
   int minPoints;       // Minimum points for a cluster to be created
   int CounterNoise=0;  // Counter for Noise Elements
   int Cluster = 0;     // Cluster index
   int h,k;             // h counter for Neighborhood Distance Calculation, k counter for files
   clock_t start, end;  // Variables for counting execution time
   char filename[40];
   int choise;// Choise holder for switch


/* -------------------------------------------------------------------------- */
  // Reading Dataset, counting n elements, assigning to X Array

 FILE* Dataset;

    printf("\n Give the DataSet file:");
    scanf("%s", filename);

  Dataset = fopen(filename, "r");
  if (!Dataset)
  {
      printf("\n There is something wrong with the Dataset file! \n\n");
      return -1;
  }
  /* -------------------------------------------------------------------------- */
printf("\n Pick the format of your data file:");
printf("\n1: Tab seperated values");
printf("\n2: Space seperated values");
printf("\n3: Comma seperated values");
printf("\nYour Choise: ");
scanf("%d",&choise );
printf("\n");


switch(choise)
{
  case 1:
    dim = getColumnsTab(Dataset); //Getting the dimensions of each Element of the Dataset
    rewind(Dataset);
      break;
  case 2:
     dim = getColumnsSpace(Dataset); //Getting the dimensions of each Element of the Dataset
     rewind(Dataset);
      break;
   case 3:
     dim = getColumnsComma(Dataset); //Getting the dimensions of each Element of the Dataset
     rewind(Dataset);
       break;
}


  n = getRows(Dataset);      // Getting the Elements of the Dataset
  rewind(Dataset);

  printf("\n Elements:%d \n", n);
  printf("\n Dimensions:%d \n", dim);


  printf("Give the amount of Minimum Points: ");
  scanf("%d",&minPoints );
  printf("Give the minimum Distance: ");
  scanf("%lf",&minDist);

  /* -------------------------------------------------------------------------- */
      // All the necessary memory allocation

         double **X;  // Array of Elements
         X = calloc(n, sizeof(double));
         for (d = 0; d < n; d++)
         X[d] = calloc(dim, sizeof(double));

         int *visited;  //0 for unvisited, 1 for visited
         visited = calloc(n,sizeof(int));

         for(i = 0; i < n; i++)
         visited[i] = 0;

         int *Noise; //Flag for points that are Noise
         Noise = calloc(n,sizeof(int));
         for(i = 0; i < n; i++)
         Noise[i] = 0;

         int *Core;  //Flag for core points,  0 for border, 1 for Core
         Core = calloc(n,sizeof(int));
         for(i = 0; i < n; i++)
         Core[i] = 0;

double *distance ;  //Array for holding Distances for the First Core point of a cluster
distance = calloc(n,sizeof(double));
for(i = 0; i < n; i++)
distance[i] = 0;

double **distance2 ;   //Array for holding Distances for each core point of a Neighborhood with Each Element of the dataset
distance2 = calloc(n,sizeof(double));
for(i = 0; i < n; i++)
distance2[i] = calloc(n,sizeof(double));


 int *temp;  //Array for holding which Elements belong to First Neighborhood
 temp = calloc(n,sizeof(int));
 for(i = 0; i < n; i++)
 temp[i] = 0;

 int *temp2; //Array for holding which Elements belong to Core Point's Neighborhoods
 temp2 = calloc(n,sizeof(int));
 for(i = 0; i < n; i++)
 temp2[i] = 0;

int *numPoints;//size of Neighborhood for first Core Point
numPoints = calloc(n,sizeof(int));
for(i = 0; i < n; i++)
numPoints[i] = 0;

int *numPointsP;//size of Neighborhood for each Core Point
numPointsP = calloc(n,sizeof(int));
for(i = 0; i < n; i++)
numPointsP[i] = 0;

//C[i][0] , First Column always holds the Cluster in which The elements are contained
      double **C;  // Array of Clusters
      C = calloc(n, sizeof(double));
      for (d = 0; d <= n; d++)
          C[d] = calloc((dim+1), sizeof(double));


/* -------------------------------------------------------------------------- */
      // Passing elements to Array X[n][dim]
      n--;

      switch(choise)
      {
        case 1:
            X = getDataTab(Dataset,n,dim,X);
        break;
        case 2:
            X = getDataSpace(Dataset,n,dim,X);
        break;
        case 3:
            X = getDataComma(Dataset,n,dim,X);
        break;
      }


//Convert the Dataset to only possitive values
// for(i = 0; i < n; i++)
//    for(d = 0; d < dim; d++)
//    X[i][d] = fabs(X[i][d]);

/* -------------------------------------------------------------------------- */

start = clock();

/*-----------------------Starting DBSCAN--------------------------------------*/

for(i = 0; i < n; i++)
{
   if(visited[i] != 1) //Checking if the Element is unvisited or not to proceed
   {
      visited[i] = 1; //If it is unvisited then mark it as visited

/* -------------------------------------------------------------------------- */
    //Finding the Neighborhood

       //Calculating the distance of Xi with each unvisited element
           for(j = 0; j < n; j++)
           {
             if(visited[j] != 1)
             {
              temp[j] = 0; // Mark temp 0 for current Point, used for reset
                 distance[j] = 0; //Reset the distance before calculating for current point
                  for(d = 0; d < dim; d++){
  //Calculating and storing distances of Xi element with the rest of the points
                 distance[j] += sqrt((X[i][d] - X[j][d])*(X[i][d] - X[j][d]));
                                          }

            //Calculating the size of the Neighborhood
                    if(distance[j] <= minDist)
                    {
  //If Current elements belongs to the Neighborhood set temp 1 and Increase size
                       temp[j] = 1;
                          numPoints[i]++;
                    }
              }
            }
  //Raising by one so that the i element to be accounted for
     numPoints[i]++;
/* -------------------------------------------------------------------------- */
  //Checking if the element is core or noise
  //if core change it to Core = 1 else mark it as noise
        //
        if(numPoints[i] < minPoints)
          {
  //If Noise then mark it as one,set core to -1 and pass it to array
  // C as -1 to first column,then pass the rest of the Element to C
            Noise[i] = 1;
            Core[i] = -1;
            CounterNoise++;
            C[i][0] = -1;
            for(d = 1; d <= dim; d++)
            C[i][d] = X[i][d-1];
          }else
          {
            //If Found as core set Core to 1 and pass it to Cluster
            Core[i] = 1;

   //Passing the Core element
            C[i][0] = Cluster;
            for(d = 1; d <= dim; d++)
            C[i][d] = X[i][d-1];
    /* -------------------------------------------------------------------------- */
    //Passing the rest of Neighborhood to array C
              for(j = i + 1; j < n; j++)
              {
      //If temp[j] = 1 means that the current element belongs to Neighborhood
      //and then you pass it to array C with the current Cluster Value to
      //it's First Column, then mark is as visited
                if(temp[j] == 1 && visited[j] != 1)
                {
                  C[j][0] = Cluster;
                  for(d = 1; d <= dim; d++)
                  C[j][d] = X[j][d-1];
                  visited[j] = 1;
                }
              }
    /* -------------------------------------------------------------------------- */
    //Finding the distance for each Element of our initial Neighborhood with each Element of the dataset
    //Each [j][h] Combination Holds the distances of Current j element with each h element
              for(j = 0; j < n; j++)
              {
                if(temp[j] == 1)
                {
                  for(h = 0; h < n; h++)
                    {
                    distance2[j][h] = 0;
                    for(d = 1; d <= dim; d++)
                      distance2[j][h] += sqrt((C[j][d] - X[h][d-1])*(C[j][d] - X[h][d-1]));
                    }
                }
              }
/* -------------------------------------------------------------------------- */
for(j = 0; j < n; j++)
{
  //If temp[j] = 1 means that the current element belongs to Neighborhood
  if(temp[j] == 1)
  {
    /* -------------------------------------------------------------------------- */
  //Calculating and Storing the Neighborhood of each Initial Neighborhood Element
     numPointsP[j] = 0;
    for(h = 0; h < n; h++)
    {
      //Reseting temp2 for current h element
      temp2[h] = 0;
      //Comparing Distance of Current h element with minDist
       if(distance2[j][h] <= minDist)
        {
          //if h element is found to belong to j's element Neighborhood
          //set temp2[h] = 1 and increase the Neighborhood's size
          temp2[h] = 1;
          numPointsP[j]++;
        }

    }
      //Raising by one so that the i element to be accounted for
      numPointsP[j]++;


/* -------------------------------------------------------------------------- */
//Checking if the current Neighborhood Element is Core or Border,if it was Noise unmark it
    if(numPointsP[j] < minPoints)
    {
         if(Noise[j] == 1)
         {
           //If it was Marked as noise but currently belongs to someone's
           //Neighborhood unmark it and set it directly as border
           //If it was Core element it would never been marked as noise before
         Core[j] = 0;
         Noise[j] = 0;
       }else
       {
       Core[j] = 0;
       }
       //Change it's first Column to current Cluster
       C[j][0] = Cluster;
    }else
    {
//if it is found as Core pass it's Neighborhood to it's cluster,
//Set Core = 1
      Core[j] = 1;
      for(h = 0; h < n; h++){
        //If temp2[h] = 1 means that element h belongs to j's Neighborhood
      if(temp2[h] == 1)
      {
      //Check if it is unvisited, if yes proceed to pass it to current Cluster
      //Then mark it as visited
        if(visited[h] != 1)
        {
          C[h][0] = Cluster;
          for(d = 1; d <= dim; d++)
          C[h][d] = X[h][d-1];
          visited[h] = 1;
        }

      }
    }
    }
  }
}

/* -------------------------------------------------------------------------- */
//After a succesfull Cluster Creation/Expansion increase Cluster by 1
              Cluster++;
          }
   }
}

//End of DBSCAN
end = clock();
/*---------------------------------------------------------------------------*/
double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;

//
// for(i = 0; i < n ; i++)
// {
// printf("\n");
// printf("%d :",i+1 );
// printf("Noise: %d ",Noise[i]);
// printf("Core: %d ",Core[i]);
//  printf("Visited: %d ",visited[i]);
//  printf("Cluster %d ",(int)C[i][0] );
//    for(d = 1; d <= dim; d++ )
//     printf("%lf ",C[i][d] );
//
//   }

printf("\n");
printf("\n");
printf("CounterNoise: %d\n",CounterNoise );
printf("\n");
printf("\n Time of Algorithm Execution: %lf \n\n",total_time);

printf("\n");

FILE** FileCluster = calloc(Cluster+1,sizeof(FILE*));
FILE** initialFile = calloc(Cluster+1,sizeof(FILE*));
FILE** finalFile = calloc(Cluster+1,sizeof(FILE*));

k = Cluster;

for(j = 0 ; j < k ; j++)
{
  char *fileName;
 fileName = calloc(n,sizeof(char));
  snprintf(fileName,n*sizeof(char),"Cluster_%d.txt",j);
   FileCluster[j] = fopen(fileName,"w");
   free(fileName);
}


for(i = 0; i < n; i++){
for( j = 0; j < k; j++){
   for(d = 1; d <= dim; d++){
       if(C[i][0] == j){
       fprintf(FileCluster[j], "%lf",C[i][d]);
       if(d <= dim - 1)
       {
         fprintf(FileCluster[j],", ");
       }
     }
     }
fprintf(FileCluster[j], "\n" );
}
}

for (j = 0; j < k; j++)
fclose(FileCluster[j]);

/* -------------------------------------------------------------------------- */
    //Creating and writing to a file the Final Noise
FILE* NoiseFile;

NoiseFile = fopen("FinalNoise.txt","w");

for(j = 0; j < n; j++){
  if(Noise[j] == 1){
 if(j != 0)
 fprintf(NoiseFile, "\n");
for(d = 1; d <= dim; d++){
fprintf(NoiseFile, "%lf",C[j][d]);
if(d < dim ){
  fprintf(NoiseFile,", ");
}
}
}
}
fclose(NoiseFile);


//Creating and writing to a file the Core Points
FILE* CoreFile;

CoreFile = fopen("FinalCore.txt","w");

for(j = 0; j < n; j++){
if(Core[j] == 1){
if(j != 0)
fprintf(CoreFile, "\n");
for(d = 1; d <= dim; d++){
fprintf(CoreFile, "%lf",C[j][d]);
if(d < dim ){
fprintf(CoreFile,", ");
}
}
}
}
fclose(CoreFile);

/* -------------------------------------------------------------------------- */
    // Deleting Empty rows and Blank rows

int MAX = n*k*dim;
char str[MAX];

for(j = 0 ; j < k ; j++)
{
char *fileName;
fileName = calloc(n,sizeof(char));
snprintf(fileName,n*sizeof(char),"Cluster_%d.txt",j);
initialFile[j] = fopen(fileName,"r");
if (!initialFile[j])
{
    printf("\n There is something wrong with the Initial file! \n\n");
    return -2;
}
free(fileName);
}

for(j = 0 ; j < k ; j++)
{
char *fileName;
fileName = calloc(n,sizeof(char));
snprintf(fileName,n*sizeof(char),"c%d.txt",j);
finalFile[j] = fopen(fileName,"w");
free(fileName);
}

for(j = 0; j < k; j++)
{
while(!feof(initialFile[j]))
{
  fgets(str,MAX,initialFile[j]);
  if(strncmp(str,"\n",1) == 0)
  {
    continue;
  }
  fputs(str,finalFile[j]);
  strncpy(str, "\0",1);
}
}

for(j = 0; j < k; j++)
fclose(initialFile[j]);

for(j = 0; j < k; j++)
fclose(finalFile[j]);

//Deleting Useless Files

for(j = 0 ; j < k ; j++)
{
char *fileName;
fileName = calloc(n,sizeof(char));
snprintf(fileName,n*sizeof(char),"Cluster_%d.txt",j);
remove(fileName);
free(fileName);
}

for(j = 60 ; j >= k ; j--)
{
char *fileName;
fileName = calloc(n,sizeof(char));
snprintf(fileName,n*sizeof(char),"c%d.txt",j);
remove(fileName);
free(fileName);
}

/* -------------------------------------------------------------------------- */

free(X);
free(C);
free(distance);
free(temp);
free(visited);
free(Noise);
free(Core);
free(temp2);
free(numPoints);
free(numPointsP);
free(distance2);
free(FileCluster);
free(finalFile);
free(initialFile);

  return 0;
}
