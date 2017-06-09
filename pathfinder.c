#include "pathfinder.h"
/*
PF_Map *InitPathfinder(Map *map, Robot *robot)
{
    PF_Map *pf = malloc(sizeof(PF_Map));

    pf->width = map->width;
    pf->height = map->height;

    //Allocation de la map
    pf->map = malloc(sizeof(char *) * height);

    for (i = 0 ; i < height ; i++)
    {
        pf->map[i] = malloc(sizeof(char) * (width+1));
        srtcpy(pf->map[i], map->map[i]);
    }

    return pf;
}
*/

void ScanMap(Map *map, Robot *robot, int dir)
{
    turn(robot, dir, MOVE_ABSOLUTE);

    MapPoint *target = getTarget(robot);
    if (robot->knownMap[target->y][target->x] != '.')//Vérifier si on est déja allé à ce point
    {
        if (moveFwd(map, robot, 1))//Si on peut y avancer...
        {
            //Exploration des cases adjacentes
            ScanMap(map, robot, LEFT);
            ScanMap(map, robot, UP);
            ScanMap(map, robot, RIGHT);
            ScanMap(map, robot, DOWN);

            //Retour au sens dans lequel il est arrivé sur cette case
            turn(robot, dir, MOVE_ABSOLUTE);
            //Demi tour
            turn(robot, RIGHT, MOVE_RELATIVE);
            turn(robot, RIGHT, MOVE_RELATIVE);
            //Avancer sur la case d'avant
            moveFwd(map, robot, 1);
        } else {
            //Il y a un mur
        }
    }
}
