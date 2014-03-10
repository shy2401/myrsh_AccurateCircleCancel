#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.hpp"
#include "pushRelabel.hpp"
#include "variable.hpp"

/* #define NODES 4 */
/* #define MIN(X,Y) ((X) < (Y) ? (X) : (Y)) */
/* #define INFINITE 10000000 */

/* BFS对图中节点进行排序,
 * 从汇点向源点使用BFS，
 * 用于PushRelabel中了list链表
 */
#define WHITE 0
#define GRAY 1
#define BLACK 2

void bfs(Edge **C, int nNodes, int sink, int *sequenceList)
{
    int *colour;
    Item u, v, *p;
    Item i = 0; 
    int k = 0;
    p = (Item *)malloc(sizeof(Item));
    colour = (int *)calloc(nNodes, sizeof(int));
    colour[sink] = GRAY;
    Queue *queue = InitQueue();
    EnQueue(queue, sink);
    do {
        while (!IsEmpty(queue)) {
            DeQueue(queue, p);
            sequenceList[k++] = v = *p;
            for (u = 0; u < nNodes; u++) {
                if (colour[u] == WHITE && C[u][v].eContent > 0) {
                    EnQueue(queue, u);
                    colour[u] = GRAY;
                }
            }
            colour[v] = BLACK;
        }
        if (colour[i] == WHITE) {
            EnQueue(queue, i);
            colour[i] = GRAY;
        }
    } while(i++ < nNodes);

    free(p);
    free(colour);
    DestroyQueue(queue);
}

/*
 * C是容量矩阵，其内容不会改变，F是流量矩阵。
 * excess是记录余留。
 */

void push(Edge **C, int **F, int *excess, int u, int v)
{
    int send = MIN(excess[u], C[u][v].eContent - F[u][v]);
    F[u][v] += send;
    F[v][u] -= send;
    excess[u] -= send;
    excess[v] += send; 
}

void relabel(Edge **C, int **F, int nNodes, int *height, int u)
{
    int v;
    int min_height = INFINITE;
    for (v = 0; v < nNodes; v++) {
        if (C[u][v].eContent - F[u][v] > 0) {
            min_height = MIN(min_height, height[v]);
            height[u] = min_height + 1;
        }
    }
}

/* /\* 将溢出顶点的所有流都排除出去。 */
/*  *\/ */
/* void discharge(Edge **C, int **F, int nNodes, int *excess,  */
/*                int *height, int *seen, int u) */
/* { */
/*     while (excess[u] > 0) { */
/*         if (seen[u] < nNodes) { */
/*             int v = seen[u]; */

/*             /\* 此处用height[u]>height[v]而不是用height[u]=height[v]+1, */
/*              * 是因为满足第一个条件C[u][v]-F[u][v]>0后， */
/*              * 如果满足height[u]>height[v]，就满足了height[u]=height[v]+1, */
/*              * height[u]>height[v]+1已经被第一个条件排除了。 */
/*              *\/ */
/*             if (C[u][v].eContent - F[u][v] > 0 && height[u] > height[v]) { */
/*                 push(C, F, excess, u, v); */
/*             } else { */
/*                 seen[u] += 1; */
/*             } */
/*         } else { */
/*             relabel(C, F, nNodes, height, u); */
/*             seen[u] = 0; */
/*         } */
/*     } */
/* } */

void discharge(Edge **C, int **F, int nNodes, int *excess,
               int *height, int *seen, int *sequenceList, int u)
{
    while (excess[u] > 0) {
        if (seen[u] < nNodes) {
            int v = sequenceList[seen[u]];
            if (C[u][v].eContent - F[u][v] > 0 && height[u] > height[v]) {
                push(C, F, excess, u, v);
            } else {
                seen[u] += 1;
            }
        } else {
            relabel(C, F, nNodes, height, u);
            seen[u] = 0;
        }
    }
}

void moveToFront(int i, int *A)
{
    int temp = A[i];
    int n;
    for (n = i; n > 0; n--) {
        A[n] = A[n-1];
    }
    A[0] = temp;
}

void pushRelabel(Edge **C, int **F, int nNodes, int source, int sink
                /* , int *maxFlow, double *probability */)
{
    int *excess, *height, *list, *seen, i, /* j, */ p;
    int *sequenceList;
    /* int  *constList; */

    excess = (int *)calloc(nNodes, sizeof(int));
    height = (int *)calloc(nNodes, sizeof(int));
    seen = (int *)calloc(nNodes, sizeof(int));

    sequenceList = (int *)calloc(nNodes, sizeof(int));
    bfs(C, nNodes, sink, sequenceList);

    list = (int *)calloc(nNodes-2, sizeof(int));
    /* constList = (int *)calloc(nNodes-2, sizeof(int)); */

    /* for (i = 0, p = 0; i < nNodes; i++) { */
    /*     if (sequenceList[i] != source && */
    /*         sequenceList[i] != sink) { */
    /*         list[p] = sequenceList[i]; */
    /*         p++; */
    /*     } */
    /* } */

    for (i = 0, p = 0; i < nNodes; i++) {
        if ((i != source) && (i != sink)) {
            list[p] = i;
            /* constList[p] = i; */
            p++;
        }
    }

    height[source] = nNodes;
    excess[source] = INFINITE;
    for (i = 0; i < nNodes; i++)
        push(C, F, excess, source, i);

    p = 0;
    while (p < nNodes-2) {
        int u = list[p];
        int old_height = height[u];
        discharge(C, F, nNodes, excess, height, seen, sequenceList, u);
        if (height[u] > old_height) {
            /* memcpy(list, constList, (nNodes -2) * sizeof(int)); */
            moveToFront(p, list);
            p = 0;
        } else {
            p += 1;
        }
    }

    /* *maxFlow = 0; */
    /* for (i = 0; i < nNodes; i++) */
    /*     *maxFlow += F[source][i]; */

    /* *probability = 1; */
    /* for (i = 0; i < nNodes; i++) */
    /*     for (j = 0; j < nNodes; j++) */
    /*         if (F[i][j] > 0) */
    /*             *probability *= C[i][j].eProbability; */

    free(excess);
    free(height);
    free(seen);
    free(sequenceList);
    free(list);

}

int getMaxFlow(int **F, int source, int nNodes)
{
    int i;
    int maxflow = 0;
    for (i = 0; i < nNodes; i++)
        maxflow += F[source][i];

    return maxflow;
}

double getProbability(Edge **C, int **F, int nNodes)
{
    int i, j;
    double probability = 1.0;
    for (i = 0; i < nNodes; i++)
        for (j = 0; j < nNodes; j++)
            if (C[i][j].eContent > 0 && F[i][j] > 0)
                probability *= C[i][j].eProbability;
    return probability;
}

/* void printMatrix(int **M) */
/* { */
/*     int i, j; */
/*     for (i = 0; i < NODES; i++) { */
/*         for (j = 0; j < NODES; j++) { */
/*             printf("%d\t", M[i][j]); */
/*         } */
/*         printf("\n"); */
/*     } */
/* } */

/* void printMatrixEdge(Edge **M) */
/* { */
/*     int i, j; */
/*     for (i = 0; i < NODES; i++) { */
/*         for (j = 0; j < NODES; j++) { */
/*             printf("%d %f\t", M[i][j].eContent, M[i][j].eProbability); */
/*         } */
/*         printf("\n"); */
/*     } */
/* } */

/* int main(void) */
/* { */
/*     int **flow, i; */
/*     Edge **capacities; */
/*     flow = (int **)calloc(NODES, sizeof(int *)); */
/*     capacities = (Edge **)calloc(NODES, sizeof(Edge *)); */
/*     for (i = 0; i < NODES; i++) { */
/*         flow[i] = (int *)calloc(NODES, sizeof(int)); */
/*         capacities[i] = (Edge *)calloc(NODES, sizeof(Edge)); */
/*     } */

/*     /\* Sample graph *\/ */
/*     /\* capacities[0][1] = 2; *\/ */
/*     /\* capacities[0][2] = 9; *\/ */
/*     /\* capacities[1][2] = 1; *\/ */
/*     /\* capacities[1][3] = 0; *\/ */
/*     /\* capacities[1][4] = 0; *\/ */
/*     /\* capacities[2][4] = 7; *\/ */
/*     /\* capacities[3][5] = 7; *\/ */
/*     /\* capacities[4][5] = 4; *\/ */

/*     /\* Sample *\/ */
/*     capacities[0][1].eContent = 4; */
/*     capacities[0][2].eContent = 3; */
/*     capacities[1][2].eContent = 2; */
/*     capacities[1][3].eContent = 4; */
/*     capacities[2][3].eContent = 5; */

/*     printf("Capacity:\n"); */
/*     printMatrixEdge(capacities); */

/*     printf("Maxflow:\n%d\n", pushRelabel(capacities, flow, 0, 3)); */
/*     printf("flows:\n"); */
/*     printMatrix(flow); */
/*     return 0; */
/* } */
