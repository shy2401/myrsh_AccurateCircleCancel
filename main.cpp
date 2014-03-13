#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sys/time.h>
#include "variable.hpp"
#include "pushRelabel.hpp"
#include "rw.hpp"
#include "circuits.hpp"
#include "circuits_cancel.hpp"
#include "negativeCommunity.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Edge **capacities;
    int **flow, maxFlow, source, sink;
    int **remain;  /* 剩余图 */
    int i;
    double probability;
    int nNodes, nEdges;  /* 图中的顶点数，边数 */
    vector<vector<int> > circuits;
    FILE *fp;
    long fpLocate;

    struct timeval start;
    struct timeval end;
    double time_use;

    if (argc != 2) {
        fprintf(stderr, "pushRelabel <input_filename> \n");
        exit(1);
    }    /* 输出是直接输出到终端，若要输出到文件中，则在终端中使用重定向 */

    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "pushRelabel: can't open %s\n", argv[1]);
        exit(2);
    }

    readNaEFromFile(fp, &nNodes, &nEdges, &source, &sink, &fpLocate);

    flow = (int **)calloc(nNodes, sizeof(int *));
    remain = (int **)calloc(nNodes, sizeof(int *));
    capacities = (Edge **)calloc(nNodes, sizeof(Edge *));
    for (i = 0; i < nNodes; ++i) {
        flow[i] = (int *)calloc(nNodes, sizeof(int));
        remain[i] = (int *)calloc(nNodes, sizeof(int));
        capacities[i] = (Edge *)calloc(nNodes, sizeof(Edge));
    }

    readFromFile(fp, capacities, nEdges, fpLocate);

    gettimeofday(&start, NULL);

    pushRelabel(capacities, flow, nNodes, source-1, sink-1);
    updateRemain(capacities, flow, remain, nNodes);

    NCC(capacities, flow, remain, nNodes, sink-1);
    
    maxFlow = getMaxFlow(flow, source-1, nNodes);
    probability = getProbability(capacities, flow, nNodes);

    gettimeofday(&end, NULL);

    time_use = (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;


    printf("%s\n", argv[1]);
    printf("maxFlow = %d, probability = %f\n", maxFlow, probability);
    printf("total time(us): %.2f\n", time_use);
    printf("----------------------------------------\n");

    exit(0);
}
