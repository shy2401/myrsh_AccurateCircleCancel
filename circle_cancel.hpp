#ifndef CIRCLE_CANCEL_H
#define CIRCLE_CANCEL_H

#include "variable.hpp"

/* void cancelConcussion(Edge **C, int nNodes); */

/* void solveCircleCancel(Edge **C, int **F, int **remain, int nNodes); */

// void updateRemain(Edge **C, int **F, int **remain, int nNodes);

void solveFAC(Edge **C, int **F, int **remain, int nNodes, int sink);

#endif
