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
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "func.h"

#define GUI_TILESIZE 16     //Taille d'une tuile
#define GUI_NB_TEXTURES 6   //Nb de textures

/*      Modes     */
#define MODE_2D 0           //Affichage 2D
#define MODE_ISOMETRIC 1    //Affichage 3D isométrique

#define TXT_CENTERED 1      //Origine du texte en son centre
#define TXT_DEFAULT 0       //Origine du texte par défaut (en haut à gauche)

/*    Textures    */
#define TX_WALL 0
#define TX_GROUND 1
#define TX_ROBOT 2
#define TX_START 3
#define TX_ARRIVAL 4
#define TX_VISITED 5

#define TX_WALL_YOFFSET 13

typedef struct GUI_Component{//Composant graphique de base
    int width;              //Largeur
    int height;             //Hauteur
    int mode;               //Mode de dessin
    int outputOffset;       //Décalage vertical
    SDL_Surface *internal;  //Surface graphique SDL
    SDL_Surface *textures[GUI_NB_TEXTURES]; //Textures
    Mix_Chunk *music;       //Musiquer de fond (samplerate : 48000, ogg ou WAV PCM)
    TTF_Font *font;         //Police utilisée pour les messages
}GUI_Component;

GUI_Component *initGUI(Map *map, int mode);                             //Initialise l'environnement graphique
SDL_Surface *OpenWindow(int width,int height, char *title, int flags);  //Ouvre une fenêtre standard de type standard

void DrawTile(GUI_Component *output, int x, int y, char type);          //Affiche une case
void DrawIsometricTile(GUI_Component *output, int x, int y, char type); //Affiche une case en utilisant la méthode de 3D isométrique
void DrawText(GUI_Component *window, TTF_Font *font, int x, int y, char *message, int mode);    //Affiche un message texte à la position et selon le mode spécifiés
void DrawMap(GUI_Component *output, Map *map, Robot *robot);            //Dessine toute la map
void DrawRobot(GUI_Component *window, Robot *robot);                    //Affiche le robot
void ClearRobot(GUI_Component *window, Robot *robot);                   //Efface le robot et laisse une trace
void RenderGUI(GUI_Component *output);                                  //Affiche les opérations de dessin précédentes

int ManageEvents(GUI_Component *window);                                //Gére les événements de l'environnement graphique
void FreeGUI(GUI_Component *window);                                    //Est censé libérer l'environnement graphique mais fait une fuite de 1.4Mo

#endif //_INTERFACE_INC_
