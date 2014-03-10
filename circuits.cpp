#include <vector>
#include <list>
#include <algorithm>
#include "circuits.hpp"

using std::vector;
using std::list;
//using std::find;

static void unblock(int u, bool *blocked, vector<list<int> > &blockedNodes)
{
  blocked[u] = false;
  while (!blockedNodes[u].empty()) {
    int w = blockedNodes[u].front();
    blockedNodes[u].pop_front();
    if (blocked[w]) 
      unblock(w, blocked, blockedNodes);
  }
}

static int getNextNeighbor(int **remain, int nNodes, int v, int n)
{
  for (int i = n + 1; i < nNodes; i++) {
    if (remain[v][i] > 0)
      return i;
  }
  return -1;
}



static bool circuit(int **remain, int nNodes, int v, int s, vector<int> &Stack,
                    bool *blocked, vector<list<int> > &blockedNodes, 
                    vector<vector<int> > &circuits)
{
  bool f;
  if (s == nNodes)
    return false;

  Stack.push_back(v);
  blocked[v] = true;

  for (int w = getNextNeighbor(remain, nNodes, v, s-1); w != -1; 
       w = getNextNeighbor(remain, nNodes, v, w)) {
    if (w == s) {
      Stack.push_back(s);
      if (Stack.size() > 3) 
        circuits.push_back(Stack);
      Stack.pop_back();
      f = true;
    } else {
      if (!blocked[w]) 
        if (circuit(remain, nNodes, w, s, Stack, blocked, blockedNodes, circuits))
          f = true;
    }
  }
  if (f) 
    unblock(v, blocked, blockedNodes);
  else {
    for (int w = getNextNeighbor(remain, nNodes, v, s-1); w != -1;
         w = getNextNeighbor(remain, nNodes, v, w)) {
      if (find(blockedNodes[w].begin(), blockedNodes[w].end(), v) != 
          blockedNodes[w].end()) {
        blockedNodes[w].push_back(v);
      }
    }
  }
  Stack.pop_back();
  return f;
}

void findCircuits(int **remain, int nNodes, vector<vector<int> > &circuits)
{
  bool *blocked;
  vector<list<int> > blockedNodes(nNodes);
  vector<int> Stack;
  blocked = new bool[nNodes];

  for (int s = 0; s < nNodes; s++) {
    for (int j = 0; j < nNodes; j++) {
      blocked[j] = false;
      blockedNodes[j].clear();
    }
    circuit(remain, nNodes, s, s, Stack, blocked, blockedNodes, circuits);
  }

  delete [] blocked;
}
