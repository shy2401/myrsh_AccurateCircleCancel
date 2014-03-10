#ifndef PUSHRELABEL_H
#define PUSHRELABEL_H

#include "variable.hpp"

/* 用于测试的 */
/* void bfs(Edge **C, int nNodes, int sink, int *sequenceList); */

/* 使用压入重标记算法计算最大流,返回值是最大流 */
void pushRelabel(Edge **C, int **F, int nNodes, int source, int sink 
                /* , int *maxFlow, double *probability */); 

int getMaxFlow(int **F, int source, int nNodes);
double getProbability(Edge **C, int **F, int nNodes);

#endif
