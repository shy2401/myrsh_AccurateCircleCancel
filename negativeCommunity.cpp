#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "circuits.hpp"
#include "circle_cancel.hpp"
#include "variable.hpp"
#include "negativeCommunity.hpp"

using std::vector;

//测试用的输出
static void printArray(int **Array, int nNodes)
{
  int i, j;

  for (i = 0; i < nNodes; i++) {
    for (j = 0; j < nNodes; j++) {
      printf("%4d", Array[i][j]);
    }
    printf("\n");
  }
}

static void circlesContainEdge(int x, int y, vector<vector<int> > &circuits,
                               vector<int> &locateInCircuits)
{
  int i, index = 0;

  for (vector<vector<int> >::iterator vvit = circuits.begin(); vvit != circuits.end(); ++vvit) {
    for (i = 0; i < (int)(*vvit).size() - 1; ++i) {
      if ((*vvit)[i] == x && (*vvit)[i+1] == y) {
        locateInCircuits.push_back(index);
        break;
      }
    }
    index++;
  }
}

// 计算除去边x->y之外的环的权值及预流
static void computeWeightAndPreflow(Edge **C, int **F, int **remain, int x, int y, vector<int> &circle, vector<double> &weight, vector<int> &preflow)
{
  int i, j;
  double WT = 0.0;
  int cLen = circle.size();
  int PF = INFINITE;

  for (i = 0; i < cLen - 1; ++i) {
    j = i+1;
    if (circle[i] == x && circle[j] == y)
      continue;
    PF = MIN(PF, remain[circle[i]][circle[j]]);
  }

  if (PF > 0) {
    for (i = 0; i < cLen - 1; ++i) {
      j = i+1;
      if (circle[i] == x && circle[j] == y)
        continue;
      if (C[circle[i]][circle[j]].eContent > 0 && F[circle[i]][circle[j]] ==0) {
        WT += -log(C[circle[i]][circle[j]].eProbability);
      }
      if (F[circle[i]][circle[j]] == -PF) {
        WT += log(C[circle[j]][circle[i]].eProbability);
      }
    }
    weight.push_back(WT);
    preflow.push_back(PF);
  }
}

static void sortWTPFLC(vector<double> &weight, vector<int> &preflow, vector<int> &locateInCircuits)
{
  int i, j;
  double tmpWT;
  int tmpPF, tmpLC;
  for (i = 0; i < (int)weight.size(); i++) {
    for (j = 1; j < (int)weight.size() - i; j++) {
      if (weight[j-1] > weight[j]) {
        tmpWT = weight[j-1];
        weight[j-1] = weight[j];
        weight[j] = tmpWT;

        tmpPF = preflow[j-1];
        preflow[j-1] = preflow[j];
        preflow[j] = tmpPF;

        tmpLC = locateInCircuits[j-1];
        locateInCircuits[j-1] = locateInCircuits[j];
        locateInCircuits[j] = tmpLC;
      }
    }
  }
}

static void setPreflowArray(Edge **C, int x, int y, vector<vector<int> > &circuits, vector<int> &locateInCircuits, vector<int> &preflow, int circleCount, int **preflowArray, int keyEdgePreflow)
{
  int i, j;

  for (i = 0; i < circleCount; i++) {
    if (keyEdgePreflow >= preflow[i]) {
      for (j = 0; j < (int)circuits[locateInCircuits[i]].size() - 1; ++j) {
        preflowArray[circuits[locateInCircuits[i]][j]][circuits[locateInCircuits[i]][j+1]] += preflow[i];
      }
    } else {
      if (C[y][x].eContent > 0) {
        for (j = 0; j < (int)circuits[locateInCircuits[i]].size() - 1; ++j) {
          preflowArray[circuits[locateInCircuits[i]][j]][circuits[locateInCircuits[i]][j+1]] += keyEdgePreflow;      
        }
      }
    }
    keyEdgePreflow -= preflow[i];
  }
}

static bool satisfyRemainCapacity(int **remain, int **preflowArray, int nNodes)
{
  int i, j;
  for (i = 0; i < nNodes; ++i) {
    for (j = 0; j < nNodes; ++j) {
      if (preflowArray[i][j] > 0 && preflowArray[i][j] > remain[i][j])
        return false;
    }
  }
  return true;
}

static double computeWeight(Edge **C, int **remain, int **preflowArray, int nNodes)
{
  int i, j;
  double weight = 0.0;

  for (i = 0; i < nNodes; ++i) {
    for (j = 0; j < nNodes; ++j) {
      if (preflowArray[i][j] > 0) {
        if (C[i][j].eContent > 0 && C[i][j].eContent == remain[i][j])
          weight -= log(C[i][j].eProbability);
        if (C[j][i].eContent > 0 && remain[i][j] == preflowArray[i][j])
          weight += log(C[j][i].eProbability);
      }
    }
  }

  return weight;
}

static void renewRemainAndFlow(int **F, int **remain, int **preflowArray, int nNodes)
{
  int i, j;

  for (i = 0; i < nNodes; ++i) {
    for (j = 0; j < nNodes; ++j) {
      //---调试用
      printf("remain\n\n");
      printArray(remain, nNodes);
      printf("flow\n\n");
      printArray(F, nNodes);
      //---
      if (preflowArray[i][j] > 0) {
        remain[i][j] -= preflowArray[i][j];
        remain[j][i] += preflowArray[i][j];
        F[i][j] += preflowArray[i][j];
        F[j][i] -= preflowArray[i][j];
      }
    }
  }
}

void setPreflowArrayZero(int **preflowArray, int nNodes)
{
  int i, j;
  for (i = 0; i < nNodes; ++i)
    for (j = 0; j < nNodes; ++j)
      preflowArray[i][j] = 0;
}


bool negativeCommunityCancel(Edge **C, int **F, int **remain, int nNodes)
{
  int i, j, k;
  vector<vector<int> > circuits;
  vector<int> locateInCircuits;
  vector<double> weight;
  vector<int> preflow;
  int **preflowArray;
  preflowArray = (int **)calloc(nNodes, sizeof(int *));
  for (i = 0; i < nNodes; ++i)
    preflowArray[i] = (int *)calloc(nNodes, sizeof(int));

  findCircuits(remain, nNodes, circuits);

  //---调试用
  printf("remain\n\n");
  printArray(remain, nNodes);
  printf("flow\n\n");
  printArray(F, nNodes);
  //---

  for (i = 0; i < nNodes; ++i) {
    for (j = 0; j < nNodes; ++j) {
      locateInCircuits.clear();
      weight.clear();
      preflow.clear();
      setPreflowArrayZero(preflowArray, nNodes);

      if (remain[i][j] > 0) {
        circlesContainEdge(i, j, circuits, locateInCircuits);
        for (vector<int>::iterator it = locateInCircuits.begin(); it != locateInCircuits.end(); ++it) {
          computeWeightAndPreflow(C, F, remain, i, j, circuits[*it], weight, preflow);
        }
        sortWTPFLC(weight, preflow, locateInCircuits);

        if (C[i][j].eContent > 0) {
          int circleCount = 0;
          int preflowCount = 0;
          for (k = 0; k < (int)weight.size(); ++k) {
            if (weight[k] < 0) {
              if ((preflowCount += preflow[k]) < remain[i][j]) {
                circleCount++;
              } else {
                circleCount++;
                break;
              }
            } else {
              break;
            }
          }
          setPreflowArray(C, i, j, circuits, locateInCircuits, preflow, circleCount, preflowArray, remain[i][j]);
          if (satisfyRemainCapacity(remain, preflowArray, nNodes)) {

            //---调试用
            printf("preflowArray\n\n");
            printArray(preflowArray, nNodes); 
            //---

            double WT = computeWeight(C, remain, preflowArray, nNodes);
            if (WT < 0) {
              //---调试用
              printf("remain\n\n");
              printArray(remain, nNodes);
              printf("flow\n\n");
              printArray(F, nNodes);
              //---

              renewRemainAndFlow(F, remain, preflowArray, nNodes);
              return true;
            }
          }
        }

        if (C[j][i].eContent > 0) {
          int circleCount = 0;
          int preflowCount = 0;
          for (k = 0; k < (int)weight.size(); ++k) {
            if ((preflowCount += preflow[i]) < remain[i][j]) {
              circleCount++;
            } else {
              circleCount++;
              break;
            }
          }

          setPreflowArray(C, i, j, circuits, locateInCircuits, preflow, circleCount, preflowArray, remain[i][j]);
          if (satisfyRemainCapacity(remain, preflowArray, nNodes)) {
            double WT = computeWeight(C, remain, preflowArray, nNodes);
            if (WT < 0) {
              renewRemainAndFlow(F, remain, preflowArray, nNodes);
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

void NCC(Edge **C, int **F, int **remain, int nNodes, int sink)
{
  do {
    // solveFAC(C, F, remain, nNodes, sink);
    ;
  }while (negativeCommunityCancel(C, F, remain, nNodes));

}