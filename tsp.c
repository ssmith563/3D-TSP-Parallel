#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define PLACES 5

int main();
char* InitiateCharArray (int);
int* InitiateIntArray (int);
char** InitiateMultiCharArray (int, int);
int GetPossibleOptions (int);
int GetRandomNumber (int);
int GetPossibleOptions (int);

int main ()
{
    char *TravledPlaces = InitiateCharArray(PLACES);
    char **TravelOptions = InitiateMultiCharArray(GetPossibleOptions(PLACES), PLACES+1);
    int  *NextPlaceOptions = InitiateIntArray(PLACES-1);
    int i, j = 0;
    bool solved = false;

    for (i = 0; i < PLACES-1; i++) {
        NextPlaceOptions[i] = i+1;
    }
    printf("NextPlaceOptions: {%d, %d, %d, %d} \n", NextPlaceOptions[0], NextPlaceOptions[1], NextPlaceOptions[2], NextPlaceOptions[3]);

    // Debug
    int FirstLocationOptions = GetPossibleOptions(PLACES)/(PLACES-1);
    printf("First: %d \n", FirstLocationOptions);
    int SecondLocationOptions = FirstLocationOptions/(PLACES-2);
    printf("Second: %d \n", SecondLocationOptions);
    int ThirdLocationOptions = SecondLocationOptions/(PLACES-3);
    printf("Third: %d \n\n", ThirdLocationOptions);

    while (!solved) {
        int size = 0;
        for (i = 0; i < PLACES-1; i++) {
            if (NextPlaceOptions[i] != -1) size++;
        }
        GetRandomNumber(size);
    }

    int NextPlace = 1;
    for (i = 0; i < GetPossibleOptions(PLACES); i++) {
        TravelOptions[i][0] = 0;
        TravelOptions[i][5] = 0;

        TravelOptions[i][1] = NextPlace;
        j++;

        if (j == FirstLocationOptions) {
            NextPlace += 1;
            j = 0;
        }
    }

    // Debug
    for (i = 0; i < GetPossibleOptions(PLACES); i++) {
        printf("{%d, %d, %d, %d, %d, %d} \n", TravelOptions[i][0], TravelOptions[i][1], TravelOptions[i][2],
                                              TravelOptions[i][3], TravelOptions[i][4], TravelOptions[i][5]);
    }
    return 0;
}

char* InitiateCharArray (int size) {
    char *TravledPlaces = (char *)malloc(size*sizeof(char));
    return TravledPlaces;
}

int* InitiateIntArray (int size) {
    int *TravledPlaces = (int *)malloc(size*sizeof(int));
    int i = 0;
    for (i = 0; i < size; i++) {
        TravledPlaces[i] = 0;
    }
    return TravledPlaces;
}

char** InitiateMultiCharArray (int sizeY, int sizeX) {
    char **TravelOptions = (char **)malloc(sizeY * sizeof(char*));
    int i;

    for (i = 0; i < sizeY; i++) {
        TravelOptions[i] = (char *)malloc(sizeX * sizeof(char));
    }
    return TravelOptions;
}

// Generates random number
int GetRandomNumber (int size) {
    time_t TimeStore;
    time(&TimeStore);
    srand((unsigned int)TimeStore);

    return (rand()%(size*size))+1;
}

int GetPossibleOptions (int places) {
    return places == 1 ? 1 : (places-1)*GetPossibleOptions(places-1);
}
