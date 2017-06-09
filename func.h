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

//Fonctions utils de terminal
#define RestoreCursorPosition() printf("\033[u")
#define SaveCursorPosition() printf("\033[s")
#define MoveCursorTo(x,y) printf("\033[%d;%dH", y+1, x+1)

typedef struct MapPoint{
    int x;
    int y;
}MapPoint;


typedef struct Robot{
    int x;          //Position x
    int y;          //Position y
    char dir;       //Direction (up, down, left, right)
    int lockDir;    //(bool)Verrouille la direction
    int angle;      //Somme des angles suivis (algorithme de Pledge)
    int steps;      //Distance parcourue par le robot
    char **knownMap;//Map telle que connue du robot
} Robot;

typedef struct Map{
    int width;
    int height;
    MapPoint start;
    MapPoint arrival;
    char **map;
}Map;

/* Fonctions utiles */
char *strfield(char *string, char separator, int field);//Extrait un champ d'une chaine délimité par un séparateur
void setTerminalColor(char *param); //Change la couleur du texte dans le terminal C'est zoli :)
void wait(unsigned int millis);  //Attend.... encore un peu.... encoooore... la descrption arrivera... attends... wait(-1)....
int waitInput(); //Attend qu'une touche soit pressée
int randInt(int minVal, int maxVal);//Tire un nb aléatoire entre les bornes précisées

#endif //_USEFULFUNCTIONS_INC_
