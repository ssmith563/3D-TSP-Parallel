#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>


void travellingSalesman(int n, int path[n], double cost[n][n], double * costSum);
void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange);
void generateDistanceCost(int n, double pointsArr[n][3], double cost[n][n]);
void generateEnergyCost(int n, double pointsArr[n][3], double cost[n][n]);
void printDistanceCostArray(int n, double arr[n][n]);
void printPathArray(int n, int arr[n]);
void printPointsArray(int n, double arr[n][3]);

int main()
{
    double xRange;
    double yRange;
    double zRange;
    int n;
    
    double timediff;
    clock_t start,end;

    //Gets user input
    printf("Enter x Range: ");
    scanf("%lf", &xRange);
    printf("\nEnter y Range: ");
    scanf("%lf", &yRange);
    printf("\nEnter z Range: ");
    scanf("%lf", &zRange);
    printf("\nEnter number of points: ");
    scanf("%d", &n);
    printf("\n");

    //generate points in array and print
    double (*pointsArr)[n] = malloc(sizeof(double[n][3]));

    generatePoints(n, pointsArr, xRange, yRange, zRange);
    //printPointsArray(n, pointsArr);
    
    
    //generate 3d distance cost and print
    double (*cost)[n] = malloc(sizeof(double[n][n]));
    generateDistanceCost(n, pointsArr, cost);
    //printDistanceCostArray(n, cost);

    //final path salesman takes
    int *path = malloc(n * sizeof *path);
    //cost of final path
    double costSum;


    start = clock();
    //main tsp function
    travellingSalesman(n, path, cost, &costSum);
    end = clock();
    timediff = (double)(end - start) / CLOCKS_PER_SEC;



    //print final path
    //printPathArray(n, path);
    
    printf("Minimum cost:\n%.6f\n", costSum);
    
    

   
    

    printf("Time: %.5f", timediff);
    printf("\n");
    free(cost);
    free(pointsArr);
    free(path);
    return 0;

}
void travellingSalesman(int n, int path[n], double cost[n][n], double * costSum)
{
	double sum = 0.0;
	
	double minCost;
    int minIndex;
    int lastNode = 0;
    path[0] = 1;

	//initialize visited nodes
    int visitedNodes[n];
    for(int i = 0; i < n; i++){
        visitedNodes[i] = 0;
    }
    visitedNodes[0] = 1;

    while(lastNode < n-1){
        minCost = LONG_MAX;
        for(int i = 0; i < n; i++){
            if((cost[path[lastNode] - 1][i] < minCost) && (path[lastNode] - 1 != i) && (visitedNodes[i] == 0)){
                minCost = cost[path[lastNode] - 1][i];
                minIndex = i;
            }
        }
        lastNode++;
        path[lastNode] = minIndex+1;
        sum += minCost;
        visitedNodes[minIndex] = 1;
        
    }

    *costSum = sum + cost[path[n-1]-1][0];
    
	
}

void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange){
    srand(0);
    double xDiv = RAND_MAX/xRange;
    double yDiv = RAND_MAX/yRange;
    double zDiv = RAND_MAX/zRange;
    for(int i = 0; i < n; i++){
        pointsArr[i][0] = rand()/xDiv;
        pointsArr[i][1] = rand()/yDiv;
        pointsArr[i][2] = rand()/zDiv;
    }
}

void generateDistanceCost(int n, double pointsArr[n][3], double cost[n][n]){
    double dist;
    double x2;
    double y2;
    double z2;
    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            if(j == i){
                cost[j][j] = 0;
            }
            x2 = pow(pointsArr[i][0] - pointsArr[j][0], 2);
            y2 = pow(pointsArr[i][1] - pointsArr[j][1], 2);
            z2 = pow(pointsArr[i][2] - pointsArr[j][2], 2);

            dist = pow(x2 + y2 + z2, .5);
            cost[i][j] = dist;
            cost[j][i] = dist;
        }
    }
}


void generateEnergyCost(int n, double pointsArr[n][3], double cost[n][n]){
     double dist_to;
     double dist_from;
     double x2;
     double y2;
     double z2;

     double elev;

     for(int i = 0; i < n; i++){
         for(int j = i; j < n; j++){
             if(j == i){
                 cost[j][j] = 0;
             }
             x2 = pow(pointsArr[i][0] - pointsArr[j][0], 2);
             y2 = pow(pointsArr[i][1] - pointsArr[j][1], 2);

             //z2 = pow(pointsArr[i][2] - pointsArr[j][2], 2);

             elev = pointsArr[j][2] - pointsArr[i][2];

             if(elev > 0){
                 z2 = pow(elev, 2);
                 dist_to = pow(x2 + y2, .5) + z2;
                 dist_from = pow(x2 + y2, .5) - z2;
             }
             else {
                 elev = elev * -1;
                 z2 = pow(elev, 0.5);
                 dist_to = pow(x2 + y2, .5) - z2;
                 dist_from = pow(x2 + y2, .5) + z2;
             }

             if(dist_from < 0) dist_from = 0;
             cost[i][j] = dist_to;
             cost[j][i] = dist_from;
         }
     }
 }


void printDistanceCostArray(int n, double arr[n][n]){
    printf("Cost array elements:\n");
    for(int i=0; i<n; i++) {
        printf("\n");
        for(int j=0;j<n;j++) {
            printf("%lf ", arr[i][j]);
        }
    }
    printf("\n\n");
}

void printPathArray(int n, int arr[n]){
    printf("Path array elements:\n");
    for(int i=0; i<n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}

void printPointsArray(int n, double arr[n][3]){
    printf("3D points:\n");
    for(int i=0; i<n; i++) {
        printf("\n");
        for(int j=0;j<3;j++) {
            printf("%lf ", arr[i][j]);
        }
    }
    printf("\n\n");
}
