#ifndef _AI_INC_
#define _AI_INC_


#ifndef _STD_INC_
#define _STD_INC_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#endif //_STD_INC_

#include "map.h"
#include "interface.h"
#include "func.h"

#define RIGHT 1
#define LEFT 3
#define UP 0
#define DOWN 2

#define ERR_MOVE_IMPOSSIBLE 0
#define ERR_MOVE_OUTOFRANGE -1

#define MOVE_ABSOLUTE 0
#define MOVE_RELATIVE 1

#define DELAY 50


/* Fonctions de gestion de l' IA du robot */
Robot *createRobot(Map *map);                       //Crée un nouveau Robot

int moveFwd(Map *map, Robot *robot, int dist);      //Avance le robot de <dist> cases (true si permis, false sinon)
void turn(Robot *robot, int dir, char mode);        //Tourne le robot sur lui même (absolu ou relatif à l'orientation courante)
void reachWall(Map *map, Robot *robot);             //Fonce droit dans le mur

MapPoint *getTarget(Robot *robot);                                  //Renvoie la case visée par le robot (celle mesurée par le capteur)
char getKnownCase(char **map, int x, int y, int width, int height); //Renvoie le contenu de la case demandée (+protection segfault)
int checkArrival(Map *map, Robot *robot);                           //Vérifie si le robot est à l'arrivée
int countNearestUnvisitedCases(Robot *robot);                       //Compte les cases non visitées a proximité

int Bot_FollowWall(GUI_Component *window, Map *map, Robot *robot);         //Robot stupide
int ScanMap(GUI_Component *window, Map *map, Robot *robot, int dir);      //Parcours récursif de la map
void displayRobotMap(Map *map, Robot *robot);       //Affiche le robot sur la map tel qu'il la connait

void FreeRobot(Robot *robot, int mapSize);          //Détruit le robot et libére ses ressources
#endif //_AI_INC_
