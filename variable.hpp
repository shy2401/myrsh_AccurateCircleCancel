#ifndef VARIABLE_H
#define VARIABLE_H

#define NODES 4 /* 顶点数 */
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define INFINITE 0x3f3f3f3f

typedef struct edge {
    int eContent;        /* 边上的容量 */
    double eProbability; /* 边上的可靠性 */
    int eLabel;          /* 边上的编号 */
} Edge;

#endif
