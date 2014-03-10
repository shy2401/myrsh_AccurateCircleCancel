#include <vector>
#include <cmath>
#include "variable.hpp"
#include "circuits_cancel.hpp"

using std::vector;

/* 求权值 */
static double computeWeight(Edge **C, int **F, int **remain, const vector<int> &circle,
                            int *preflow)
{
    int i, j;
    double weight = 0.0;
    int cLen = circle.size();

    /* 求出预留 */
    *preflow = INFINITE;
    for (i = 0; i < cLen-1; i++) {
        j = i+1;
        *preflow = MIN(*preflow, remain[circle[i]][circle[j]]);
    }

    /* 求权值 */
    if (*preflow > 0) {
        for (i = 0; i < cLen-1; i++) {
            j = i+1;
            if (C[circle[i]][circle[j]].eContent > 0 && 
                F[circle[i]][circle[j]] ==0) {
                weight += -log(C[circle[i]][circle[j]].eProbability);
            }
            if (F[circle[i]][circle[j]] == -(*preflow)) {
                weight += log(C[circle[j]][circle[i]].eProbability);
            }
        }
    }
    return weight;
}

static void updateFlowRemain(int **F, int **Remain, const vector<int> &circle,
                             int preflow)
{
    int i, j;
    int cLen = circle.size();

    for (i = 0; i < cLen-1; i++) {
        j = i+1;
        F[circle[i]][circle[j]] += preflow;
        F[circle[j]][circle[i]] -= preflow;
        Remain[circle[i]][circle[j]] -= preflow;
        Remain[circle[j]][circle[i]] += preflow;
    }

}

/* 更新剩余图remain */
void updateRemain(Edge **C, int **F, int **remain, int nNodes)
{
    int i, j;
    for (i = 0; i < nNodes; i++)
        for (j = 0; j < nNodes; j++)
            remain[i][j] = C[i][j].eContent - F[i][j];
}

bool NC_cancel(Edge **C, int **F, int **remain, int nNodes, vector<vector<int> > &circuits)
{
  bool existNC = false;
  int preflow;

  for (vector<vector<int> >::iterator vvit = circuits.begin(); vvit != circuits.end(); 
       ++vvit) {
    if (computeWeight(C, F, remain, *vvit, &preflow) < 0) {
      updateFlowRemain(F, remain, *vvit, preflow);
      existNC = true;
    }
  }

  return existNC;
}
