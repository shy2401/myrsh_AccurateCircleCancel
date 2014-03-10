#ifndef NEGATIVECOMMUNITY_HPP
#define NEGATIVECOMMUNITY_HPP

#include "variable.hpp"

void NCC(Edge **C, int **F, int **remain, int nNodes, int sink);

bool negativeCommunityCancel(Edge **C, int **F, int **remain, int nNodes);


#endif
