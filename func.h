#ifndef _USEFULFUNCTIONS_INC_
#define _USEFULFUNCTIONS_INC_


#ifndef _STD_INC_
#define _STD_INC_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#endif //_STD_INC_

/*** Constantes des couleurs de la console ***/
#define CL_RESET "0"
#define CL_FLASH "5"
#define CL_HICONRAST "1"
#define CL_INVERT "7"

#define CL_BLACK "30"
#define CL_RED "31"
#define CL_GREEN "32"
#define CL_YELLOW "33"
#define CL_BLUE "34"
#define CL_MAGENTA "35"
#define CL_CYAN "36"
#define CL_WHITE "37"

#define BK_BLACK "40"
#define BK_RED "41"
#define BK_GREEN "42"
#define BK_YELLOW "43"
#define BK_BLUE "44"
#define BK_MAGENTA "45"
#define BK_CYAN "46"
#define BK_WHITE "47"
/*********************************************/

#define DELAY 50    //Délai entre deux étapes

//Fonctions utiles pour l'affichage sur terminal non graphique
#define RestoreCursorPosition() printf("\033[u")
#define SaveCursorPosition() printf("\033[s")
#define MoveCursorTo(x,y) printf("\033[%d;%dH", y+1, x+1)

typedef struct MapPoint{//Point 2D
    int x;          //Coord. x
    int y;          //Coord. y
}MapPoint;


typedef struct Robot{//Robot
    int x;          //Position x
    int y;          //Position y
    char dir;       //Direction (up, down, left, right)
    int lockDir;    //(bool)Verrouille la direction
    int angle;      //Somme des angles suivis (algorithme de Pledge)
    int steps;      //Distance parcourue par le robot
    char **knownMap;//Map telle que connue du robot
} Robot;

typedef struct Map{//Map
    int width;          //Largeur de la map
    int height;         //Hauteur de la map
    MapPoint start;     //Point de départ
    MapPoint arrival;   //Point d'arrivée
    char **map;         //Tableau de char * contenant la map
}Map;

/* Fonctions utiles */
char *strfield(char *string, char separator, int field);//Extrait un champ d'une chaine délimité par un séparateur
void setTerminalColor(char *param);                     //Change la couleur du texte dans le terminal C'est zoli :)
void wait(unsigned int millis);                         //Attend.... encore un peu.... encoooore... la descrption arrivera... attends... wait(-1)....
int waitInput();                                        //Attend qu'une touche soit pressée
char *readString(char *Out,int MaxLen); //Attend la saisie d'une ligne et retire l'\n de fin
int randInt(int minVal, int maxVal);                    //Tire un nb aléatoire entre les bornes précisées

#endif //_USEFULFUNCTIONS_INC_
