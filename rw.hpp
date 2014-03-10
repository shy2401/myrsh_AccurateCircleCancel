#ifndef RWFROMFILE_H
#define RWFROMFILE_H

#include "variable.hpp"

/* 从文件中读入图的顶点数及边数 */
void readNaEFromFile(FILE *fp, int *nNodes, int *nEdges, int *source, 
                     int *sink, long *fpLocate);

/* 从文件中读入变得容量、概率及标签，存放在数组C中 */
void readFromFile(FILE *fp, Edge **C, int nEdges, long fpLocate);

/* 输出结构数组capacities */
void printCPL(Edge **C, int nNodes);

/* print flow */
void printFlow(int **flow, int nNodes);

#endif
