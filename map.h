#ifndef _MAP_INC_
#define _MAP_INC_


#ifndef _STD_INC_
#define _STD_INC_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#endif //_STD_INC_

#include "func.h"


/* Fonctions de gestion de la map */
Map *LoadMap(const char *file);         //Charge la map a partir d'un fichier
void PrintMap(Map *map);         //Affiche la map (terminal)

void sfrMap(char **map, int size);
void FreeMap(Map *map);
#endif //_MAP_INC_
