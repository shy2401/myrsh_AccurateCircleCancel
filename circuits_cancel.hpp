#ifndef CIRCUITS_CANCEL_HPP
#define CIRCUITS_CANCEL_HPP

#include <vector>
#include "variable.hpp"
using std::vector;

bool NC_cancel(Edge **C, int **F, int **remain, int nNodes, 
               vector<vector<int> > &circuits);

void updateRemain(Edge **C, int **F, int **remain, int nNodes);

#endif
