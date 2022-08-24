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
void* travellingSalesman(void* args);

//done
void generatePoints(int n, double pointsArr[n][3], double xRange, double yRange, double zRange);
void generateDistanceCost(int n, double pointsArr[n][3], double** cost);

//not implemented
void printDistanceCostArray(int n, double arr[n][n]);
void printPathArray(int n, int arr[n]);
void printPointsArray(int n, double arr[n][3]);

struct arg_struct {
    int n;
    int* path;
    double** cost;
    double costSum;
    int thread_count;
    long id;
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
    
    

    for(thread = 0; thread < thread_count; thread++){
        args.id = thread;
        pthread_create(&thread_handles[thread], NULL, &travellingSalesman, (void *)&args);
    }

    for(thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }

    end = clock();
    timediff = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.5f", timediff);
    printf("\n");

    for(int i = 0; i < n; i++){
        free(cost[i]);
    }
    free(cost);
    free(pointsArr);
    free(path);


    pthread_exit(NULL);
     
    
    return 0;
}


void* travellingSalesman(void* arguments)
{
    
    struct arg_struct *args = arguments;
	double sum = 0.0;
    int minIndex;
    args->path[0] = 1;
    

	//initialize visited nodes
    int visitedNodes[args->n];
    for(int i = 0; i < args->n; i++){
        visitedNodes[i] = 0;
    }
    visitedNodes[0] = 1;

    double minc;
    int mini;

    int quotient = args->n/args->thread_count;
    int remainder = args->n%args->thread_count;
    int my_n_count;
    int my_first_i;
    int my_last_i;

    double temp;
    

    long my_rank = args->id;
    //printf("%ld", my_rank);
    
    if(my_rank < remainder){//splits n evenly among processes
        my_n_count = quotient + 1;
        my_first_i = my_rank*my_n_count;
    }else{
        my_n_count = quotient;
        my_first_i = my_rank*my_n_count + remainder;
    }       
    my_last_i = my_first_i + my_n_count;

    double** costPtr = args->cost;
    double tester;
    for(int j = 0; j < args->n-1; j++){
        minc = LONG_MAX;
        mini = -1;
        for(int i = my_first_i; i < my_last_i; i++){


            printf("testing...\n");
            tester = *((*costPtr+0)+0);
            printf("%lf\n", tester );
            tester = *((*costPtr+0)+1);
            printf("%lf\n", tester );
            tester = *((*costPtr+0)+2);
            printf("%lf\n", tester );
            tester = *((*costPtr+1)+0);
            printf("%lf\n", tester );
            tester = *((*costPtr+1)+1);
            printf("%lf\n", tester );
            tester = *((*costPtr+1)+2);
            printf("%lf\n", tester );
            //seg faults on this line from accessing cost
            if((  *((*costPtr+args->path[j] - 1)+i) < minc) && (args->path[j] - 1 != i) && (visitedNodes[i] == 0)){
                //printf("3\n");
                minc = *((*costPtr+args->path[j] - 1)+i);
                mini = i;
            }

        }
        //printf("done\n");
        if (my_rank != 0) { 
            /* Send message to process 0 */
             

        }else{ 
            minIndex = mini;
            temp = LONG_MAX;
            if(mini == -1){
            }
            else if(*((*costPtr+args->path[j] - 1)+mini) < temp){
                minIndex = mini;
                temp = *((*costPtr+args->path[j] - 1)+minIndex);
            }

            for (int q = 1; q < args->thread_count; q++) {
                /* Receive message from process q */
                
                if(mini == -1){
                }
                else if( *((*costPtr+args->path[j] - 1)+mini) < temp){
                    minIndex = mini;
                    temp = *((*costPtr+args->path[j] - 1)+minIndex);
                }
                
            }
            args->path[j+1] = minIndex+1;
            sum += *((*costPtr+args->path[j] - 1)+minIndex);
            visitedNodes[minIndex] = 1;
        }
        //MPI_Bcast(path, n, MPI_INT, 0, MPI_COMM_WORLD);
        //MPI_Bcast(visitedNodes, n, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    if(my_rank == 0){
        args->costSum = sum + args->cost[args->path[args->n-1]-1][0];
    }
    //MPI_Bcast(costSum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
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
