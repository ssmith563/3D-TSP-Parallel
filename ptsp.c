#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>

//wip
void travellingSalesman(int n, int path[n], double cost[n][n], double * costSum);

//done
void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange);
void generateDistanceCost(int n, double pointsArr[n][3], double cost[n][n]);

//not implemented
void printDistanceCostArray(int n, double arr[n][n]);
void printPathArray(int n, int arr[n]);
void printPointsArray(int n, double arr[n][3]);


void * wThread(void* tid){

    long* myID = (long*) tid;
    printf("Thread: %ld\n", *myID);
    

}

int main(){

    int n;
    double xRange;
    double yRange;
    double zRange;

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
    

    double (*pointsArr)[n] = malloc(sizeof(double[n][3]));
    double (*cost)[n] = malloc(sizeof(double[n][n]));
    int *path = malloc(n * sizeof *path);

    generatePoints(n, pointsArr, xRange, yRange, zRange);
    generateDistanceCost(n, pointsArr, cost);

    double timediff;
    clock_t start,end;
    start = clock();

    pthread_t tid0;
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_t tid5;

    pthread_t* pthreads[] = {&tid0, &tid1, &tid2, &tid3, &tid4, &tid5};

    for(int i = 0; i < 6; i++){
   
        //travellingSalesman();
        pthread_create(pthreads[i], NULL, wThread, (void*)pthreads[i]);

    }

    printf("Time: %.5f", timediff);
    printf("\n");

    end = clock();
    timediff = (double)(end - start) / CLOCKS_PER_SEC;

    free(cost);
    free(pointsArr);
    free(path);

    pthread_exit(NULL);
     
    
    return 0;
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


/*
void updateRoute(int route){

    pthread_mutex_lock(&bestMutex);
    if(bestRoute(route)){

        cpRoute(tour, bestRoute);
        addRoute(bestRoute, startNode);

    }

    pthread_mutex_unlock(&bestMutex);

}

*/
