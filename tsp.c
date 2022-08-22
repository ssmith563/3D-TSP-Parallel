#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>


void travellingSalesman(int n, double path[n], double cost[n][n], double * costSum);
void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange);
void generateDistanceCost(int n, double pointsArr[n][3], double cost[n][n]);
void printCostArray(int n, double arr[n][n]);
void printPathArray(int n, double arr[n]);
void printPointsArray(int n, double arr[n][3]);

int main()
{
    double xRange;
    double yRange;
    double zRange;
    int n;
    
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
    double pointsArr[n][3];
    generatePoints(n, pointsArr, xRange, yRange, zRange);
    printPointsArray(n, pointsArr);
    
    //generate 3d distance cost and print
    double cost[n][n];
    generateDistanceCost(n, pointsArr, cost);
    printCostArray(n, cost);

    //final path salesman takes
    double path[n];
    //cost of final path
    double costSum;

    //main tsp function
    travellingSalesman(n, path, cost, &costSum);

    //print final path
    printPathArray(n, path);
    
    printf("Minimum cost:\n%lf\n", costSum);
    
    return 0;

}

void travellingSalesman(int n, double path[n], double cost[n][n], double * costSum){

    //replace with greedy alg
    for(int i = 0; i < n; i++){
        path[i] = 0;
    }

    *costSum = 0;
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

void printCostArray(int n, double arr[n][n]){
    printf("Cost array elements:\n");
    for(int i=0; i<n; i++) {
        printf("\n");
        for(int j=0;j<n;j++) {
            printf("%lf ", arr[i][j]);
        }
    }
    printf("\n\n");
}

void printPathArray(int n, double arr[n]){
    printf("Path array elements:\n");
    for(int i=0; i<n; i++) {
        printf("%lf ", arr[i]);
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