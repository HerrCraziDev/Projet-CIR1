#ifndef _PATHFINDER_INC_
#define _PATHFINDER_INC_


#ifndef _STD_INC_
#define _STD_INC_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#endif //_STD_INC_

#include "map.h"
#include "ai.h"
#include "interface.h"
/*
typedef struct{
    int width;
    int height;
    char **map;         //Copie de la map
    char **distances;
}PF_Map;

typedef struct{
    int x;
    int y;
    int cost;
    int heuristic;
}PF_Point;

typedef struct PF_History{
    int x;
    int y;
    int visited;

    struct PF_History *from;
}PF_History;*/

/* Algorithme de pathfinding */
void ScanMap(Map *map, Robot *robot, int dir);


#endif //_PATHFINDER_INC_
