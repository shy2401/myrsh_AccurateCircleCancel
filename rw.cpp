#include <stdio.h>
#include <stdlib.h>
#include "rw.hpp"
#include "variable.hpp"

void readNaEFromFile(FILE *fp, int *nNodes, int *nEdge, 
                     int *source, int *sink, long *fpLocate)
{
    if (fscanf(fp, " g # 1") != 0) {
        perror("rw.c1: readNaEFromFile: fscanf error");
        exit(3);
    }    
    if (fscanf(fp, " s %d t %d", source, sink) != 2) {
        perror("rw.c2: readNaEFromFile: fscanf error");
        exit(3);
    }
    if (fscanf(fp, " s %d %d", nNodes, nEdge) != 2){
        perror("rw.c3: readNaEFromFile: fscanf error");
        exit(3);
    }        
    if ((*fpLocate = ftell(fp)) < 0) {
        perror("rw.c4: readNaEFromFile: ftell error");
        exit(3);
    }
}

void readFromFile(FILE *fp, Edge **C, int nEdges, long fpLocate)
{
    int i, u, v;

    if (fseek(fp, fpLocate, SEEK_SET) != 0) {
        perror("rw.c: readFromFile: fseek error");
        exit(4);
    }

    for (i = 0; i < nEdges; i++) {
        if (fscanf(fp, " e %d %d", &u, &v) != 2) {
            perror("rw.c: readFromFile: fscanf error");
            exit(4);
        }
        if (fscanf(fp, " %d %lf %d", &C[u-1][v-1].eContent, 
                   &C[u-1][v-1].eProbability, &C[u-1][v-1].eLabel) != 3) {
            perror("rw.c: readFromFile: fscanf error");
            exit(4);
        }
    }
}

void printCPL(Edge **C, int nNodes)
{
    int i, j;

    printf("Content\n");
    printf("%4d", 0);
    for (i = 0; i < nNodes; i++)
        printf("%4d", i+1);
    printf("\n");

    for (i = 0; i < nNodes; i++) {
        printf("%4d", i+1);
        for (j = 0; j < nNodes; j++) {
            printf("%4d", C[i][j].eContent);
        }
        printf("\n");
    }

    printf("Probability\n");
    printf("%5d", 0);
    for (i = 0; i < nNodes; i++)
        printf("%5d", i+1);
    printf("\n");

    for (i = 0; i < nNodes; i++) {
        printf("%5d", i+1);
        for (j = 0; j < nNodes; j++) {
            printf("%5.2f", C[i][j].eProbability);
        }
        printf("\n");
    }
    printf("Label\n");
    for (i = 0; i < nNodes; i++) {
        for (j = 0; j < nNodes; j++) {
            printf("%4d", C[i][j].eLabel);
        }
        printf("\n");
    }
}

void printFlow(int **flow, int nNodes)
{
    int i, j;
    printf("Flow\n");
    printf("%4d", 0);
    for (i = 0; i < nNodes; i++)
        printf("%4d", i+1);
    printf("\n");

    for (i = 0; i < nNodes; i++) {
        printf("%4d", i+1);
        for (j = 0; j < nNodes; j++) {
            printf("%4d", flow[i][j]);
        }
        printf("\n");
    }
}
