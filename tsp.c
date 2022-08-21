#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>



void travellingSalesman(int n, int path[n], int cost[n][n], int * costSum);
void printCostArray(int n, int arr[n][n]);
void printPathArray(int n, int arr[n]);

int main ()
{
    //will replace with arguments
    int n = 4;
    
    //will replace with function call that calculates distance cost between 3d points
    int cost[n][n];
    cost[0][0] = 0;
    cost[0][1] = 4;
    cost[0][2] = 1;
    cost[0][3] = 3;

    cost[1][0] = 4;
    cost[1][1] = 0;
    cost[1][2] = 2;
    cost[1][3] = 1;

    cost[2][0] = 1;
    cost[2][1] = 2;
    cost[2][2] = 0;
    cost[2][3] = 5;

    cost[3][0] = 3;
    cost[3][1] = 1;
    cost[3][2] = 5;
    cost[3][3] = 0;

    
    //prints cost array
    printCostArray(n, cost);

    //final path salesman takes
    int path[n];
    //cost of final path
    int costSum;

    //main tsp function
    travellingSalesman(n, path, cost, &costSum);

    //print final path
    printPathArray(n, path);
    
    printf("%d\n", costSum);
    
    return 0;

}

void travellingSalesman(int n, int path[n], int cost[n][n], int * costSum){

    //replace with greedy alg
    for(int i = 0; i < n; i++){
        path[i] = 0;
    }

    *costSum = 0;
}

void printCostArray(int n, int arr[n][n]){
    printf("Two Dimensional array elements:\n");
    for(int i=0; i<n; i++) {
        printf("\n");
        for(int j=0;j<n;j++) {
            printf("%d ", arr[i][j]);
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