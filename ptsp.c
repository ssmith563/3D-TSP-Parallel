#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>

//wip
void* travellingSalesman(void* args);

//done
void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange);
void generateDistanceCost(int n, double pointsArr[n][3], double** cost);

//not implemented
void printDistanceCostArray(int n, double arr[n][n]);
void printPathArray(int n, int arr[n]);
void printPointsArray(int n, double arr[n][3]);

pthread_mutex_t lock;
pthread_mutex_t barrier;

struct arg_struct {
    int n;
    int* path;
    int* visitedNodes;
    double** cost;
    double costSum;
    int thread_count;
    int id;
    double mincost;
    int minindex;
    int counter;
};

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    long thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    int n;
    double costSum = 0;
    double xRange = 10;
    double yRange = 10;
    double zRange = 10;

    printf("Enter x Range: ");
    fflush( stdout );
    //scanf("%lf", &xRange);
    printf("\nEnter y Range: ");
    fflush( stdout );
    //scanf("%lf", &yRange);
    printf("\nEnter z Range: ");
    fflush( stdout );
    //scanf("%lf", &zRange);
    printf("\nEnter number of points: ");
    fflush( stdout );
    scanf("%d", &n);
    printf("\n");
    

    double (*pointsArr)[n] = malloc(sizeof(double[n][3]));
    //double (*cost)[n] = malloc(sizeof(double[n][n]));
    int *path = malloc(n * sizeof *path);
    int *visitedNodes = malloc(n * sizeof *path);
    for(int i = 0; i < n; i++){
        visitedNodes[i] = 0;
    }
    visitedNodes[0] = 1;

    double** cost = malloc(n*sizeof(double*));
    for(int i = 0; i < n; i++){
        cost[i] = malloc(n*sizeof(double));
    }

    generatePoints(n, pointsArr, xRange, yRange, zRange);
    generateDistanceCost(n, pointsArr, cost);

    double timediff;
    clock_t start,end;
    start = clock();

    struct arg_struct args;
    //struct arg_struct *args = arguments;
    args.n = n;
    args.path = path;
    args.cost = cost;
    args.costSum = costSum;
    args.thread_count = thread_count;
    args.mincost = LONG_MAX;
    args.minindex = -1;
    args.counter = 0;
    args.visitedNodes = visitedNodes;
    
    

    for(thread = 0; thread < thread_count; thread++){
        args.id = (int)thread;
        pthread_create(&thread_handles[thread], NULL, &travellingSalesman, (void *)&args);
    }

    for(thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }

    end = clock();
    timediff = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.5f", timediff);
    printf("\n");
    printf("Cost: %lf\n", args.costSum);

    for(int i = 0; i < n; i++){
        free(cost[i]);
    }
    free(cost);
    free(pointsArr);
    free(path);


    pthread_exit(NULL);
     
    
    return 0;
}

int min(int num1, int num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}

uint64_t gettid() {
    pthread_t ptid = pthread_self();
    uint64_t threadId = 0;
    memcpy(&threadId, &ptid, min(sizeof(threadId), sizeof(ptid)));
    return threadId;
}


void *travellingSalesman(void* arguments)
{
    
    struct arg_struct *args = arguments;
	//double sum = 0.0;
    //int minIndex;
    args->path[0] = 1;


    double minc;
    int mini;

    int quotient = args->n/args->thread_count;
    int remainder = args->n%args->thread_count;
    int my_n_count;
    int my_first_i;
    int my_last_i;

    
    int my_rank = args->id;
    


    //printf("RANK = %d", my_rank);

    if(my_rank < remainder){//splits n evenly among processes
        my_n_count = quotient + 1;
        my_first_i = my_rank*my_n_count;
    }else{
        my_n_count = quotient;
        my_first_i = my_rank*my_n_count + remainder;
    }       
    my_last_i = my_first_i + my_n_count;

    
    //double tester;
    double** ct = args->cost;
    /**
    printf("testing...\n");
    tester = ct[0][0];
    printf("%lf ", tester);
    tester = ct[0][1];
    printf("%lf ", tester);
    tester = ct[0][2];
    printf("%lf ", tester);
    tester = ct[0][3];
    printf("%lf \n", tester);
    tester = ct[1][0];
    printf("%lf ", tester);
    tester = ct[1][1];
    printf("%lf ", tester);
    tester = ct[1][2];
    printf("%lf ", tester);
    tester = ct[1][3];
    printf("%lf \n", tester);
    tester = ct[2][0];
    printf("%lf ", tester);
    tester = ct[2][1];
    printf("%lf ", tester);
    tester = ct[2][2];
    printf("%lf ", tester);
    tester = ct[2][3];
    printf("%lf \n", tester);
    tester = ct[3][0];
    printf("%lf ", tester);
    tester = ct[3][1];
    printf("%lf ", tester);
    tester = ct[3][2];
    printf("%lf ", tester);
    tester = ct[3][3];
    printf("%lf ", tester);
    printf("\nEnd testing\n");

    printf("%d\n", my_rank);
    */

    for(int j = 0; j < args->n-1; j++){
        minc = LONG_MAX;
        mini = -1;
        int pth = args->path[j] - 1;
        for(int i = my_first_i; i < my_last_i; i++){
            if( (ct[pth][i] < minc) && (pth != i) && (args->visitedNodes[i] == 0)){
                minc = ct[pth][i];
                mini = i;
            }
            
        }

        pthread_mutex_lock(&lock);
        if(minc < args->mincost){
            //printf("enter mutex 1");
            args->mincost = minc;
            args->minindex = mini;
        }
        pthread_mutex_unlock(&lock);


        pthread_mutex_lock(&barrier);
        args->counter++;
        pthread_mutex_unlock(&barrier);
        while(args->counter < args->thread_count){

        }



        if (my_rank == 0) { 
            args->path[j+1] = args->minindex+1;
            args->costSum += args->mincost;//ct[pth][args->minindex];
            args->visitedNodes[args->minindex] = 1;
            args->counter = 0;
            args->mincost = LONG_MAX;
            args->minindex = -1;
        }
    }
    
    if(my_rank == 0){
        int new = args->n-1;
        int ptt = args->path[new]-1;

        args->costSum += ct[ptt][0];  
    }
    
    return NULL;
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

void generateDistanceCost(int n, double pointsArr[n][3], double** cost){

    double dist;
    double x2;
    double y2;
    double z2;

    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            if(j == i){
                cost[j][j] = 0;
                //*((*cost + j)+j) = 0;
            }
            x2 = pow(pointsArr[i][0] - pointsArr[j][0], 2);
            y2 = pow(pointsArr[i][1] - pointsArr[j][1], 2);
            z2 = pow(pointsArr[i][2] - pointsArr[j][2], 2);

            dist = pow(x2 + y2 + z2, .5);
            //*((*cost + i)+j) = dist;
            //*((*cost + j)+i) = dist;
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
