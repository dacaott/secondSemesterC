#define _CRT_SECURE_NO_WARNINGS
#include "heap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define UNOWNED (-1)
#define INVALID_COST (-1)

int createGraph(FILE* inputFile, int*** graph, int* citiesAmount, int* waysAmount)
{
    fscanf(inputFile, "%d %d", citiesAmount, waysAmount);
    *graph = (int**)malloc(*citiesAmount * sizeof(int*));
    if (*graph == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (int i = 0; i < *citiesAmount; i++) {
        (*graph)[i] = (int*)malloc(*citiesAmount * sizeof(int));
        if ((*graph)[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free((*graph)[j]);
            }
            free(*graph);
            perror("Memory allocation failed");
            return 1;
        }
        for (int j = 0; j < *citiesAmount; j++) {
            (*graph)[i][j] = INVALID_COST;
        }
    }

    for (int i = 0; i < *waysAmount; i++) {
        int city1, city2, cost;
        fscanf(inputFile, "%d %d %d", &city1, &city2, &cost);
        (*graph)[city1 - 1][city2 - 1] = cost;
        (*graph)[city2 - 1][city1 - 1] = cost;
    }

    return 0;
}

int initOwnersOfCities(int** ownersOfCities, int citiesAmount)
{
    *ownersOfCities = (int*)malloc(citiesAmount * sizeof(int));
    if (*ownersOfCities == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (int i = 0; i < citiesAmount; i++) {
        (*ownersOfCities)[i] = UNOWNED;
    }
    return 0;
}

int capitalProccessing(int** graph, int citiesAmount, int capitalCity, int* ownersOfCities, PriorityQueue* pq)
{
    ownersOfCities[capitalCity] = capitalCity;
    for (int j = 0; j < citiesAmount; j++) {
        if (graph[capitalCity][j] != INVALID_COST) {
            Edge edge = { j, graph[capitalCity][j], capitalCity };
            enqueue(pq, edge);
        }
    }
    return 0;
}

int manageCapitals(int** graph, int citiesAmount, int capitalAmount, int* capitalCities, int* ownersOfCities, PriorityQueue** pqs)
{
    for (int i = 0; i < capitalAmount; i++) {
        capitalProccessing(graph, citiesAmount, capitalCities[i], ownersOfCities, pqs[i]);
    }
    return 0;
}

int getCapitals(FILE* inputFile, int* capitalAmount, int** capitalCities, PriorityQueue*** pqs)
{
    fscanf(inputFile, "%d", capitalAmount);
    *pqs = (PriorityQueue**)malloc(*capitalAmount * sizeof(PriorityQueue*));
    if (pqs == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (int i = 0; i < *capitalAmount; i++) {
        (*pqs)[i] = createPriorityQueue();
        if ((*pqs)[i] == NULL) {
            for (int j = 0; j < i; j++) {
                freePriorityQueue((*pqs)[j]);
            }
            free(*pqs);
            perror("Memory allocation failed");
            return 1;
        }
    }

    *capitalCities = (int*)malloc(*capitalAmount * sizeof(int));
    if (*capitalCities == NULL) {
        for (int i = 0; i < *capitalAmount; i++) {
            freePriorityQueue((*pqs)[i]);
        }
        free(*pqs);
        perror("Memory allocation failed");
        return 1;
    }
    for (int i = 0; i < *capitalAmount; i++) {
        fscanf(inputFile, "%d", &((*capitalCities)[i]));
        (*capitalCities)[i]--;
    }
    return 0;
}

int edgeProcess(int** graph, int citiesAmount, int* ownersOfCities, PriorityQueue* pq)
{
    if (isEmpty(pq)) {
        return 0;
    }

    Edge edge = dequeue(pq);

    while (edge.city != -1 && ownersOfCities[edge.city] != UNOWNED) {
        if (isEmpty(pq))
            return 0;
        edge = dequeue(pq);
    }

    if (edge.city != -1 && ownersOfCities[edge.city] == UNOWNED) {
        ownersOfCities[edge.city] = edge.owner;
        for (int j = 0; j < citiesAmount; j++) {
            if (graph[edge.city][j] != INVALID_COST && ownersOfCities[j] == UNOWNED) {
                Edge newEdge = { j, graph[edge.city][j], edge.owner };
                enqueue(pq, newEdge);
            }
        }
    }
    return 0;
}

bool isAllPQEmpty(PriorityQueue** pqs, int capitalAmount)
{
    for (int i = 0; i < capitalAmount; i++) {
        if (!isEmpty(pqs[i])) {
            return false;
        }
    }
    return true;
}

int buildCountries(int** graph, int citiesAmount, int capitalAmount, int* capitalCities, int* ownersOfCities, PriorityQueue** pqs)
{
    bool* isPQEmpty = (bool*)malloc((sizeof(bool) * capitalAmount));
    if (isPQEmpty == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    for (int i = 0; i < capitalAmount; i++) {
        isPQEmpty[i] = false;
    }
    bool isEnd = false;
    while (!isEnd) {
        for (int i = 0; i < capitalAmount; i++) {
            if (isPQEmpty[i]) {
                continue;
            }
            if (edgeProcess(graph, citiesAmount, ownersOfCities, pqs[i]) != 0) {
                free(isPQEmpty);
                return 1;
            }
            if (isEmpty(pqs[i])) {
                isPQEmpty[i] = true;
            }
        }
        isEnd = isAllPQEmpty(pqs, capitalAmount);
    }

    free(isPQEmpty);
    return 0;
}

int printCountries(const int* ownersOfCities, int citiesAmount, int capitalAmount, int* capitalCities, const char* outputFilePath)
{
    FILE* outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    for (int i = 0; i < capitalAmount; i++) {
        fprintf(outputFile, "%d:", capitalCities[i] + 1);
        for (int j = 0; j < citiesAmount; j++) {
            if (ownersOfCities[j] == capitalCities[i]) {
                fprintf(outputFile, " %d", j + 1);
            }
        }
        fprintf(outputFile, "\n");
    }
    fclose(outputFile);
    return 0;
}

void clearWorkspace(int** graph, int citiesAmount, int capitalAmount, PriorityQueue** pqs, int* capitalCities, int* ownersOfCities, FILE* inputFile)
{
    for (int i = 0; i < citiesAmount; i++) {
        free(graph[i]);
    }
    free(graph);
    for (int i = 0; i < capitalAmount; i++) {
        freePriorityQueue(pqs[i]);
    }
    free(pqs);
    free(capitalCities);
    free(ownersOfCities);
    fclose(inputFile);
}

int solution(const char* inputFilePath, const char* outputFilePath)
{

    FILE* inputFile = fopen(inputFilePath, "r");

    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    int** graph;
    int citiesAmount, waysAmount;
    if (createGraph(inputFile, &graph, &citiesAmount, &waysAmount) != 0) {
        perror("Error creating graph");
        fclose(inputFile);
        return 1;
    }

    int* ownersOfCities;
    if (initOwnersOfCities(&ownersOfCities, citiesAmount) != 0) {
        perror("Error creating graph");
        for (int i = 0; i < citiesAmount; i++) {
            free(graph[i]);
        }
        free(graph);
        fclose(inputFile);
        return 1;
    }

    int capitalAmount;
    int* capitalCities;
    PriorityQueue** pqs;
    if (getCapitals(inputFile, &capitalAmount, &capitalCities, &pqs) != 0) {
        for (int i = 0; i < citiesAmount; i++) {
            free(graph[i]);
        }
        free(graph);
        free(ownersOfCities);
        fclose(inputFile);
        return 1;
    }

    if (manageCapitals(graph, citiesAmount, capitalAmount, capitalCities, ownersOfCities, pqs) != 0) {
        clearWorkspace(graph, citiesAmount, capitalAmount, pqs, capitalCities, ownersOfCities, inputFile);
        return 1;
    }

    if (buildCountries(graph, citiesAmount, capitalAmount, capitalCities, ownersOfCities, pqs) != 0) {
        clearWorkspace(graph, citiesAmount, capitalAmount, pqs, capitalCities, ownersOfCities, inputFile);
        return 1;
    }

    if (printCountries(ownersOfCities, citiesAmount, capitalAmount, capitalCities, outputFilePath) != 0) {
        clearWorkspace(graph, citiesAmount, capitalAmount, pqs, capitalCities, ownersOfCities, inputFile);
        return 1;
    }

    clearWorkspace(graph, citiesAmount, capitalAmount, pqs, capitalCities, ownersOfCities, inputFile);
    return 0;
}