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
   int cluster = 0;     // Cluster index
   int h,k;             // h counter for Neighborhood Distance Calculation, k counter for files
   clock_t start, end;  // Variables for counting execution time
   char filename[40];
   char c;
   int choise = 0;
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




 dim = getColumns(Dataset);

  n = getRows(Dataset);      // Getting the Elements of the Dataset
  rewind(Dataset);

  printf("\n Elements:%d \n", n-1);
  printf("\n Dimensions:%d \n", dim);
n--;

  printf("Give the amount of Minimum Points: ");
  scanf("%d",&minPoints );
  printf("Give the minimum Distance: ");
  scanf("%lf",&minDist);

  /* -------------------------------------------------------------------------- */
      // All the necessary memory allocation

         double **X;  // Array of Elements
         X = (double **)calloc(n, sizeof(double *));
         for (d = 0; d < n; d++)
         X[d] = (double *)calloc(dim, sizeof(double));

         int *Cluster;
         Cluster = (int *)calloc(n,sizeof(int));

         for(i = 0; i < n; i++)
         Cluster[i] = -1;

         int *visited; //Array for knowning which elements are visited and which are not
         visited =(int *) calloc(n,sizeof(int));

         for(i = 0; i < n; i++)
         visited[i] = 0; //0 for unvisited , 1 for visited

         double *distance; //array for holding distances of element i with each db element
         distance =(double *) calloc(n,sizeof(double));

         int *numPoints; //Array for holding the Neighborhood's size of each i element
         numPoints =(int *) calloc(n,sizeof(int));

         int *belong; //array for holding boolean value of an element belong to a Neighborhood
         belong = (int *)calloc(n,sizeof(int));

         int *nBelong; //array for holding boolean value of an element belong to a Neighborhood
         nBelong = (int *)calloc(n,sizeof(int));

         int *Noise; //Flag for points that are Noise
         Noise =(int *) calloc(n,sizeof(int));
         for(i = 0; i < n; i++)
         Noise[i] = 0;

         int *Core;  //Flag for core points,  0 for border, 1 for Core
         Core = (int *) calloc(n,sizeof(int));
         for(i = 0; i < n; i++)
         Core[i] = 0;

         double **distance2 ;   //Array for holding Distances for each core point of a Neighborhood with Each Element of the dataset
         distance2 = (double **) calloc(n,sizeof(double *));
         for(i = 0; i < n; i++)
         distance2[i] =(double *) calloc(n,sizeof(double));

/* -------------------------------------------------------------------------- */
      // Passing elements to Array X[n][dim]



            X = getData(Dataset,n,dim,X);



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
             if(j != i)
             {
              belong[j] = 0; // Mark temp 0 for current Point, used for reset
                 distance[j] = 0; //Reset the distance before calculating for current point
                  for(d = 0; d < dim; d++)
                  {
  //Calculating and storing distances of Xi element with the rest of the points
                 distance[j] += (X[j][d] - X[i][d])*(X[j][d] - X[i][d]);
                  }
                  distance[j] = sqrt(distance[j]);

            //Calculating the size of the Neighborhood
                    if(distance[j] <= minDist)
                    {
  //If Current elements belongs to the Neighborhood set temp 1 and Increase size
                       belong[j] = 1;
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
          Cluster[i] = -1;

          }else
          {
            //If Found as core set Core to 1 and pass it to Cluster
            Core[i] = 1;

   //Passing the Core element
            Cluster[i] = cluster;

    /* -------------------------------------------------------------------------- */
    //Passing the rest of Neighborhood to array C
              for(j = 0; j < n; j++)
              {
      //If belong[j] = 1 means that the current element belongs to Neighborhood
      //and then you pass it to array C with the current Cluster Value to
      //it's First Column, then mark is as visited
                if(belong[j] == 1 && visited[j] != 1)
                {
                  Cluster[j] = cluster;

                  visited[j] = 1;
                }
              }
    /* -------------------------------------------------------------------------- */
    //Finding the distance for each Element of our initial Neighborhood with each Element of the dataset
    //Each [j][h] Combination Holds the distances of Current j element with each h element
              for(j = 0; j < n; j++)
              {
                if(belong[j] == 1)
                {
                  for(h = 0; h < n; h++)
                    {
                    distance2[j][h] = 0;
                    for(d = 0; d < dim; d++)
                      distance2[j][h] += (X[h][d] - X[j][d])*(X[h][d] - X[j][d]);

                      distance2[j][h] = sqrt(distance2[j][h]);
                    }

                }
              }
/* -------------------------------------------------------------------------- */
for(j = 0; j < n; j++)
{
  //If belong[j] = 1 means that the current element belongs to Neighborhood
  if(belong[j] == 1)
  {
    /* -------------------------------------------------------------------------- */
  //Calculating and Storing the Neighborhood of each Initial Neighborhood Element
     numPoints[j] = 0;
    for(h = 0; h < n; h++)
    {
      //Reseting temp2 for current h element
      nBelong[h] = 0;
      //Comparing Distance of Current h element with minDist
       if(distance2[j][h] <= minDist)
        {
          //if h element is found to belong to j's element Neighborhood
          //set nBelong[h] = 1 and increase the Neighborhood's size
          nBelong[h] = 1;
          numPoints[j]++;
        }

    }
      //Raising by one so that the i element to be accounted for
      // numPoints[j]++;


/* -------------------------------------------------------------------------- */
//Checking if the current Neighborhood Element is Core or Border,if it was Noise unmark it
    if(numPoints[j] < minPoints)
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
       Cluster[j] = cluster;
    }else
    {
//if it is found as Core pass it's Neighborhood to it's cluster,
//Set Core = 1
      Core[j] = 1;
      for(h = 0; h < n; h++){
        //If nBelong[h] = 1 means that element h belongs to j's Neighborhood
      if(nBelong[h] == 1)
      {
      //Check if it is unvisited, if yes proceed to pass it to current Cluster
      //Then mark it as visited
        if(visited[h] != 1)
        {
          Cluster[h] = cluster;

          visited[h] = 1;
        }

      }
    }
    }
  }
}

/* -------------------------------------------------------------------------- */
//After a succesfull Cluster Creation/Expansion increase Cluster by 1
              cluster++;
          }
   }
}
/*---------------------------------------------------------------------------*/
//End of DBSCAN
end = clock();
/*---------------------------------------------------------------------------*/
double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;


// for(i = 0; i < n ; i++)
// {
// printf("\n");
// printf("%d :",i+1 );
// printf("Noise: %d ",Noise[i]);
// printf("Core: %d ",Core[i]);
//  printf("Visited: %d ",visited[i]);
//  printf("Cluster %d ",Cluster[i] );
//    for(d = 0; d < dim; d++ )
//     printf("%lf ",X[i][d] );
//
//   }
CounterNoise = 0;
for(i = 0; i < n; i++)
{
  if(Noise[i] == 1)
  {
    CounterNoise++;
  }
}

printf("\n");
printf("Clusters : %d \n",cluster );
printf("\n");
printf("CounterNoise: %d\n",CounterNoise );
printf("\n");
printf("\n Time of Algorithm Execution: %lf \n\n",total_time);

printf("\n");

FILE* NoiseFile;

NoiseFile = fopen("FinalNoise.txt","w");

for(j = 0; j < n; j++){
  if(Noise[j] == 1)
  {
    for(d = 0; d < dim; d++)
    fprintf(NoiseFile, "%lf ",X[j][d] );

    fprintf(NoiseFile, "\n");
  }
}
fclose(NoiseFile);


for(j = 200 ; j >= cluster ; j--)
{
char *fileName;
fileName = calloc(n,sizeof(char));
snprintf(fileName,n*sizeof(char),"C%d.txt",j);
remove(fileName);
free(fileName);
}

/* -------------------------------------------------------------------------- */
for(i = 0; i < n; i++)
free(X[i]);
free(X);
free(Cluster);
free(visited);
free(Noise);
free(nBelong);
free(belong);
free(numPoints);
free(distance);
free(Core);
for(i = 0; i < n; i++)
free(distance2[i]);
free(distance2);


  return 0;
}
