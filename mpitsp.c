#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>


void travellingSalesman(int n, int path[n], double cost[n][n], double * costSum, int comm_sz, int my_rank);
void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange);
void generateDistanceCost(int n, double pointsArr[n][3], double cost[n][n]);
void printDistanceCostArray(int n, double arr[n][n]);
void printPathArray(int n, int arr[n]);
void printPointsArray(int n, double arr[n][3]);

int main()
{
    int comm_sz;
    int my_rank;
    int n;
    double xRange;
    double yRange;
    double zRange;
    
    //initialize parallel threads
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //Gets user input
    if(my_rank == 0){
        printf("Enter x Range: ");
        fflush( stdout );
        scanf("%lf", &xRange);
        printf("\nEnter y Range: ");
        fflush( stdout );
        scanf("%lf", &yRange);
        printf("\nEnter z Range: ");
        fflush( stdout );
        scanf("%lf", &zRange);
        printf("\nEnter number of points: ");
        fflush( stdout );
        scanf("%d", &n);
        printf("\n");
    }

    //broadcast user input to all threads
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&xRange, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&yRange, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&zRange, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //initialize 3D point array, cost array, and solution path array on heap 
    double (*pointsArr)[n] = malloc(sizeof(double[n][3]));
    double (*cost)[n] = malloc(sizeof(double[n][n]));
    int *path = malloc(n * sizeof *path);
    
    if(my_rank == 0){
        //generate points in array and print
        generatePoints(n, pointsArr, xRange, yRange, zRange);
        //printPointsArray(n, pointsArr);

        //generate 3d distance cost and print
        generateDistanceCost(n, pointsArr, cost);
        //printDistanceCostArray(n, cost);
    }
    
    //broadcast 3D points array and cost array to all threads
    MPI_Bcast(pointsArr, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(cost, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
    double costSum;
    double start;
    double finish;
    double elapsed;

    //Get start time
    if(my_rank == 0){
        start = MPI_Wtime();
    }

    //TSP function
    travellingSalesman(n, path, cost, &costSum, comm_sz, my_rank);
    
    //Print total time TSP takes
    if(my_rank == 0){
        finish = MPI_Wtime();
        elapsed = finish - start;
        printf("Exec took %f seconds\n", elapsed);
        printf("Minimum cost:\n%lf\n", costSum);
    }

    //delete arrays
    free(cost);
    free(pointsArr);
    free(path);

    //end threads
    MPI_Finalize();
    return 0;

}
void travellingSalesman(int n, int path[n], double cost[n][n], double * costSum, int comm_sz, int my_rank)
{
	double sum = 0.0;
    int minIndex;
    path[0] = 1;

	//initialize visited nodes
    int visitedNodes[n];
    for(int i = 0; i < n; i++){
        visitedNodes[i] = 0;
    }
    visitedNodes[0] = 1;

    //individual thread min count and index
    double minc;
    int mini;

    int quotient = n/comm_sz;
    int remainder = n%comm_sz;
    int my_n_count;
    int my_first_i;
    int my_last_i;

    //splits n evenly among processes
    if(my_rank < remainder){
        my_n_count = quotient + 1;
        my_first_i = my_rank*my_n_count;
    }else{
        my_n_count = quotient;
        my_first_i = my_rank*my_n_count + remainder;
    }       
    my_last_i = my_first_i + my_n_count;

    double tempMin;
    
    //loop that iterates through path array to set each index to solution calculated 
    for(int j = 0; j < n-1; j++){
        minc = LONG_MAX;
        mini = -1;
        //parallelized loop that finds closest available node to previous node in path array
        for(int i = my_first_i; i < my_last_i; i++){
            if((cost[path[j] - 1][i] < minc) && (path[j] - 1 != i) && (visitedNodes[i] == 0)){
                minc = cost[path[j] - 1][i];
                mini = i;
            }
        }
        
        if (my_rank != 0) { //Send min index to process 0
            MPI_Send(&mini, 1, MPI_INT, 0, 0,
            MPI_COMM_WORLD); 
        }else{ 
            //set proc 0 min index
            minIndex = mini;
            tempMin = LONG_MAX;
            if(mini == -1){
            }
            else if(cost[path[j] - 1][mini] < tempMin){
                minIndex = mini;
                tempMin = cost[path[j] - 1][minIndex];
            }
            //recieve local index from each process
            for (int q = 1; q < comm_sz; q++) {
                MPI_Recv(&mini, 1, MPI_INT, q,
                0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //replace tempMin if index is min is lower
                if(mini == -1){
                }
                else if(cost[path[j] - 1][mini] < tempMin){
                    minIndex = mini;
                    tempMin = cost[path[j] - 1][minIndex];
                }
                
            }
            //update solution path, total sum, and visited nodes
            path[j+1] = minIndex+1;
            sum += cost[path[j] - 1][minIndex];
            visitedNodes[minIndex] = 1;
        }
        //broadcast path and visited nodes array
        MPI_Bcast(path, n, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(visitedNodes, n, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    //update and broadcast final cost sum
    if(my_rank == 0){
        *costSum = sum + cost[path[n-1]-1][0];
    }
    MPI_Bcast(costSum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
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
