#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <sys/time.h>
#include "variable.hpp"
#include "circle_cancel.hpp"

/* 单环消去算法 */

/* #define BOOL int  */
/* #define TRUE 1 */
/* #define FALSE 0 */

/* 消去震荡 */
/* void cancelConcussion(Edge **C, int nNodes) */
/* { */
/*     int i, j; */
/*     for (i = 0; i < nNodes; i++) { */
/*         for (j = 0; j < nNodes; j++) { */
/*             if (C[i][j].eContent > 0 && C[j][i].eContent > 0 */
/*                 && C[i][j].eContent > C[j][i].eContent) { */
/*                 C[i][j].eContent -= C[j][i].eContent; */
/*                 C[j][i].eContent = 0; */
/*                 C[j][i].eProbability = 0.0; */
/*             } else if (C[i][j].eContent > 0 && C[j][i].eContent > 0 */
/*                        && C[j][i].eContent > C[j][i].eContent) { */
/*                 C[j][i].eContent -= C[i][j].eContent; */
/*                 C[i][j].eContent = 0; */
/*                 C[i][j].eProbability = 0.0; */
/*             } */
/*         } */
/*     } */
/* } */


/* 求强联通分量 */
void getAllCircle(int **remain, int *Belong, int *DFN, int *LOW, int *Stack,
                  int *Stop, bool *instack, int *Bcnt, int *Dindex,
                  int nNodes, int u)
{
    int v;
    DFN[u] = LOW[u] = ++*Dindex;
    instack[u] = true;
    Stack[(*Stop)++] = u;

    for (v = 0; v < nNodes; v++) {
        if (remain[u][v] > 0) {
            if (!DFN[v]) {
                getAllCircle(remain, Belong, DFN, LOW, Stack, Stop, instack, 
                             Bcnt, Dindex, nNodes, v);
                if (LOW[v] < LOW[u])
                    LOW[u] = LOW[v];
            } else if (instack[v] && DFN[v] < LOW[u])
                LOW[u] = DFN[v];
        }
    }
    if (DFN[u] == LOW[u]) {
        (*Bcnt)++;
        do {
            v = Stack[--*Stop];
            instack[v] = false;
            Belong[v] = *Bcnt;
        } while (v != u);
    }
}

void solveGetAllCircle(int **remain, int *Belong, int *Bcnt, int nNodes)
{
    int *DFN, *LOW;  /* 定义DFN(u)为节点u搜索的次序编号(时间戳)，
                      *  Low(u)为u或u的子树能够追溯到的最早的栈中节点的次序号 
                      */
    int *Stack, *Stop;
    bool *instack;
    int *Dindex;
    int i;

    DFN = (int *)calloc(nNodes, sizeof(int));
    LOW = (int *)calloc(nNodes, sizeof(int));
    Stack = (int *)calloc(nNodes, sizeof(int));
    Stop = (int *)malloc(sizeof(int));
    Dindex = (int *)malloc(sizeof(int));
    instack = (bool *)calloc(nNodes, sizeof(bool));

    *Stop = *Bcnt = *Dindex = 0;
    memset(Belong, 0, sizeof(int) * nNodes);

    for (i = 0; i < nNodes; i++) {
        if (!DFN[i]) {
            getAllCircle(remain, Belong, DFN, LOW, Stack, Stop, instack, 
                         Bcnt, Dindex, nNodes, i);
        }
    }

    free(DFN);
    free(LOW);
    free(Stack);
    free(Stop);

    free(Dindex);
    free(instack);
}

/* void reverse(int *circle, int elementNum); */
/* double computeWeight(Edge **C, int **F, int **remain, int *circle, int kk, */
/*                      int *preflow); */
/* void updateFRemain(int **F, int **remain, int *circle, int kk, int *preflow); */

/* int existCircle(Edge **C, int **F, int **remain, int *Belong, int *Bcnt, */
/*                int nNodes) */
/* { */
/*     int eC = FALSE;  /\* 判断是否存在环 *\/ */
/*     /\* int cnt = 0; /\\* 统计强联通分量中的顶点的个数 *\\/ *\/ */
/*     int i, j, k, ki, kj, kk = 0; */
/*     int *stconn, scspt = 0;   /\* 存放强连通分量及数组中的下标*\/ */
/*     int *circle;   /\* 存放环 *\/ */
/*     int *stack, stop = 0; /\* 栈，用于将强联通分量分解成一个一个环 *\/ */
/*     int *inStack; */
/*     int *visit; /\* 用于指示强连通分量中的点是否遍历过 *\/ */
/*     int *preflow; /\* 用于存放预流 *\/ */

/*     stconn = (int *)calloc(nNodes, sizeof(int)); */
/*     circle = (int *)calloc(nNodes, sizeof(int)); */
/*     stack = (int *)calloc(nNodes, sizeof(int)); */
/*     inStack = (int *)calloc(nNodes, sizeof(int)); */
/*     visit = (int *)calloc(nNodes, sizeof(int)); */
/*     preflow = (int *)malloc(sizeof(int)); */
/*     *preflow = INFINITE; */

/*     for (i = 1; i <= *Bcnt; i++) { */
/*         for (j = 0; j < nNodes; j++) { */
/*             if (Belong[j] == i) { */
/*                 /\* cnt++; *\/ */
/*                 stconn[scspt++] = j; */
/*             } */
/*         } */
/*         /\* stack = (int *)calloc(scspt, sizeof(int)); *\/ */
/*         /\* circle = (int *)calloc(scspt, sizeof(int)); *\/ */
/*         /\* inStack = (int *)calloc(scspt, sizeof(int)); *\/ */
/*         /\* stconn[scspt] = -1; /\\* 表示数组中存放的元素结束 *\\/ *\/ */
/*         if (scspt > 2 /\* cnt > 2 *\/) { */
/*             for (k = 0; k < scspt; k++) { */
/*                 if (!visit[k]) { */
/*                     ki = k; */
/*                 label: */
/*                     if (!inStack[stconn[ki]]) { */
/*                         stack[stop++] = stconn[ki]; */
/*                         visit[ki] = TRUE; */
/*                         inStack[stconn[ki]] = TRUE; */
/*                     } */
/*                     for (kj = 0; kj < scspt; kj++) { */
/*                         if (!inStack[stconn[kj]] &&  */
/*                             remain[stconn[ki]][stconn[kj]] > 0) { */
/*                             stack[stop++] = stconn[kj]; */
/*                             visit[kj] = TRUE; */
/*                             inStack[stconn[kj]] = TRUE; */
/*                             ki = kj; */
/*                             goto label; */
/*                         } else if (inStack[stconn[kj]] &&  */
/*                                    remain[stconn[ki]][stconn[kj]] > 0) { */
/*                             if (stack[stop -2] != stconn[kj]) { */
/*                                 do { */
/*                                     circle[kk++] = stack[stop-1]; */
/*                                     /\* inStack[stack[stop-1]] = FALSE; *\/ */
/*                                 } while (stack[--stop] != stconn[kj]); */
/*                                 /\* 将circle数组倒置下, 得到的顺序就是流的方向 *\/ */
/*                                 reverse(circle, kk); */
/*                                 /\* computeWeight *\/ */
/*                                 if (computeWeight(C, F, remain, circle, kk, */
/*                                                   preflow) < 0) { */
/*                                     updateFRemain(F, remain, circle, kk,  */
/*                                                   preflow); */
/*                                     eC = TRUE; */
/*                                 } */
/*                                 stop = 0; */
/*                                 kk = 0; */
/*                                 *preflow = INFINITE; */
/*                                 memset(inStack, 0, nNodes * sizeof(int)); */
/*                                 break; */
/*                             } /\* end if *\/ */
/*                         } /\* end else if *\/ */
/*                     } /\* end for *\/ */
/*                 } /\* end if *\/ */
/*             } /\* end for *\/ */
/*         } /\* end if *\/ */

/*         scspt = 0;  /\* 一次循环后scspt归零,以便下次循环继续使用 *\/ */
/*         /\* free(stack); *\/ */
/*         /\* free(circle); *\/ */
/*     } /\* end for *\/ */

/*     free(stconn); */
/*     free(stack); */
/*     free(circle); */
/*     free(inStack); */
/*     free(visit); */
/*     free(preflow); */

/*     return eC; */
/* } */

/* void solveCircleCancel(Edge **C, int **F, int **remain, int nNodes) */
/* { */
/*     int *Belong, *Bcnt; */
/*     int i; */

/*     Belong = (int *)calloc(nNodes, sizeof(int)); */
/*     Bcnt = (int *)malloc(sizeof(int)); */
/*     *Bcnt = 0; */

/*     do { */
/*         solveGetAllCircle(remain, Belong, Bcnt, nNodes); */
/*         for (i = 0; i < nNodes; i++) */
/*             printf("%d\t", Belong[i]); */
/*         printf("\n"); */
/*     } while (existCircle(C, F, remain, Belong, Bcnt, nNodes)); */

/*     free(Belong); */
/*     free(Bcnt); */
/* } */

/* void reverse(int *circle, int elementNum) */
/* { */
/*     int i; */
/*     int temp; */
/*     for (i = 0; i <= (elementNum-1) / 2; i++) { */
/*         temp = circle[i]; */
/*         circle[i] = circle[elementNum-1-i]; */
/*         circle[elementNum-1-i] = temp; */
/*     } */
/* } */

/* 返回u的第一个邻接点 */
int getFirstNeighbor(int **remain, int nNodes, int u)
{
    int i;
    for (i = 0; i < nNodes; i++)
        if (remain[u][i] > 0)
            return i;
    return -1;
}

/* 返回u的邻接点v的下一个邻接点 */
int getNextNeighbor(int **remain, int nNodes, int u, int v)
{
    int i;
    for (i = v+1; i < nNodes; i++)
        if (remain[u][i] > 0)
            return i;
    return -1;
}

/* 返回顶点u在栈中的位置 */
int contain(int *stack, int stop, int u)
{
    int i;
    for (i = 0; i < stop; i++)
        if (stack[i] == u)
            return i;
    return -1;
}

/* 更新剩余图remain */
// static void updateRemain(Edge **C, int **F, int **remain, int nNodes)
// {
//     int i, j;
//     for (i = 0; i < nNodes; i++)
//         for (j = 0; j < nNodes; j++)
//             remain[i][j] = C[i][j].eContent - F[i][j];
// }

double computeWeight(Edge **C, int **F, int **remain, int *circle, int cLen,
                     int *preflow)
{
    int i, j;
    double weight = 0.0;

    /* 求出预留 */
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

void updateFlow(int **F, int *circle, int cLen, int *preflow)
{
    int i, j;
    for (i = 0; i < cLen-1; i++) {
        j = i+1;
        F[circle[i]][circle[j]] += *preflow;
        F[circle[j]][circle[i]] -= *preflow;
    }
}

void updateFlowRemain(int **F, int **Remain, int *circle, int cLen, 
                      int *preflow)
{
    int i, j;
    for (i = 0; i < cLen-1; i++) {
        j = i+1;
        F[circle[i]][circle[j]] += *preflow;
        F[circle[j]][circle[i]] -= *preflow;
        Remain[circle[i]][circle[j]] -= *preflow;
        Remain[circle[j]][circle[i]] += *preflow;
    }

}

/* 如果数组F改变则返回true,否则返回false */
/* int  */
void findAllCircle(Edge **C, int **F, int **remain, int *stack, int *sTop,
                   int nNodes, int sink)
{
    int u = sink; /* 表示当前的顶点 */
    /* bool ret = false;  */
    int *circle, cLen;
    int *preflow;

    // 显示每次消去一环所消耗的时间
    // struct timeval NC_begin;
    // struct timeval NC_end;
    // int NC_count = 0;
    // double time;

    circle = (int *)calloc(nNodes+1, sizeof(int));
    /* cLen = 0; */
    preflow = (int *)malloc(sizeof(int));
    /* *preflow = INFINITE; */

 labRedo:

    // 显示消去一环的时间消耗
    // gettimeofday(&NC_begin, NULL);

    for (;;) {
        while (u != -1) {
            if (contain(stack, *sTop, u) == -1) {
                stack[(*sTop)++] = u;
                u = getFirstNeighbor(remain, nNodes, u);
            } else {
                int v = contain(stack, *sTop, u);
                stack[(*sTop)++] = u;
                cLen = 0;
                while (v < *sTop) 
                    circle[cLen++] = stack[v++];

                /* int i; */
                /* for (i = 0; i < cLen; i++) */
                /*     printf("%d\t", circle[i]); */
                /* printf("\n"); */

                *preflow = INFINITE;
                /* 此处将cLen > 4改为cLen > 3 */
                if (cLen > 3 &&
                    computeWeight(C, F, remain, circle, cLen, preflow) < 0) {
                    /* updateFlow(F, circle, cLen, preflow); */
                    updateFlowRemain(F, remain, circle, cLen, preflow);
                    *sTop = 0;
                    u = circle[0];
                    cLen = 0;
                    // 显示消去一环的时间消耗
                    // NC_count++;
                    // gettimeofday(&NC_end, NULL);
                    // time = (NC_end.tv_sec-NC_begin.tv_sec)*1000000 + NC_end.tv_usec-NC_begin.tv_usec;
                    // printf("%d\t%15.2f\n", NC_count, time);
                    goto labRedo;
                    /* ret = true; */
                }
                break;
            }
        }
        if (*sTop != 0) {
            int v = stack[--(*sTop)];
            if (*sTop != 0) {
                u = stack[(*sTop)-1];
                u = getNextNeighbor(remain, nNodes, u, v);
            } else 
                goto labelReturn;
                /* return ret; */
        } else
            goto labelReturn;
            /* return ret; */
    }
 labelReturn:

    // 显示消去一环的时间消耗
    // gettimeofday(&NC_end, NULL);
    // time = (NC_end.tv_sec-NC_begin.tv_sec)*1000000 + NC_end.tv_usec-NC_begin.tv_usec;
    // printf("end\t%15.2f\n", time);

    free(circle);
    free(preflow);
    /* return ret; */
}

/* 处理所有的负权环 */
void solveFAC(Edge **C, int **F, int **remain, int nNodes, int sink)
{
    int *stack, sTop;
    stack = (int *)calloc(nNodes+1, sizeof(int));
    sTop = 0;

    /* while (findAllCircle(C, F, remain, stack, &sTop, nNodes)) { */
    /*     updateRemain(C, F, remain, nNodes); */
    /* } */

    findAllCircle(C, F, remain, stack, &sTop, nNodes, sink);

    free(stack);
}

/* int main(void) */
/* { */
/*     int **remain; */
/*     int *Belong; */
/*     int *Bcnt; */
/*     int nNodes; */
/*     int i; */

/*     nNodes = 6; */
/*     remain = (int **)calloc(nNodes, sizeof(int *)); */
/*     for (i = 0; i < nNodes; i++) { */
/*         remain[i] = (int *)calloc(nNodes, sizeof(int)); */
/*     } */

/*     Belong = (int *)calloc(nNodes, sizeof(int)); */
/*     Bcnt = (int *)malloc(sizeof(int)); */
/*     *Bcnt = 0; */

/*     remain[0][1] = 1; */
/*     remain[0][2] = 1; */
/*     remain[1][3] = 1; */
/*     remain[2][3] = 1; */
/*     remain[2][4] = 1; */
/*     remain[3][0] = 1; */
/*     remain[3][5] = 1; */
/*     remain[4][5] = 1; */

/*     solveGetAllCircle(remain, Belong, Bcnt, nNodes); */

/*     for (i = 0; i < nNodes; i++) */
/*         printf("%d\t", Belong[i]); */
/*     printf("\n"); */
/*     printf("%d\n", *Bcnt); */

/*     exit(0); */
/* } */
