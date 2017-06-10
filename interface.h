#ifndef _INTERFACE_INC_
#define _INTERFACE_INC_


#ifndef _STD_INC_
#define _STD_INC_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#endif //_STD_INC_

#include <SDL/SDL.h>
#include "func.h"

#define GUI_TILESIZE 15 //Taille d'une tuile

#define MODE_2D 0
#define MODE_ISOMETRIC 1

typedef struct GUI_Component{//Composant graphique de base
    int width;              //Largeur
    int height;             //Hauteur
    int mode;               //Mode de dessin
    int outputOffset;       //Décalage vertical
    SDL_Surface *internal;  //Surface graphique SDL
}GUI_Component;

GUI_Component *initGUI(Map *map, int mode);                             //Initialise l'environnement graphique
SDL_Surface *OpenWindow(int width,int height, char *title, int flags);  //Ouvre une fenêtre standard de type standard

void DrawTile(GUI_Component *output, int x, int y, char type);            //Affiche une case
void DrawMap(GUI_Component *output, Map *map);                          //Dessine toute la map
void DrawRobot(GUI_Component *window, Robot *robot);                    //Affiche le robot
void ClearRobot(GUI_Component *window, Robot *robot);                   //Efface le robot et laisse une trace
void RenderGUI(GUI_Component *output);                                  //Affiche les opérations de dessin précédentes

int ManageEvents(GUI_Component *window);                                //Gére les événements de l'environnement graphique
void FreeGUI(GUI_Component *window);                                    //Est censé libérer l'environnement graphique mais fait une fuite de 1.4Mo

#endif //_INTERFACE_INC_
