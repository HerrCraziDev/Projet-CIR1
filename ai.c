#include "ai.h"

/*******************************************************************/
/*createRobot(Map *map)                                             */
/*Crée un nouveau robot.                                            */
/*Paramétres :                                                      */
/*  Map *map : une Map précédemment chargée depuis un fichier       */
/*Retour :                                                          */
/*  Robot *(void) : un Robot. Se référer à func.h pour la doc. sur  */
/*                  le struct Robot                                 */
/********************************************************************/
Robot *createRobot(Map *map)
{
    Robot *robot = malloc(sizeof(Robot));
    //Initialisation des membres (cf func.h pour la documentation sur le struct Robot)
    robot->x = map->start.x;
    robot->y = map->start.y;
    robot->dir = RIGHT;
    robot->steps = 0;
    robot->angle = 0;

    //Allocation de la copie de map connue du robot
    char **knownMap = malloc(sizeof(char *) * map->height);
    int i,j;

    for (i = 0 ; i < map->height ; i++)
    {
        knownMap[i] = malloc(sizeof(char) * (map->width+1));
        for (j=0 ; j < map->width ; j++)
            knownMap[i][j] = ' ';
    }

    robot->knownMap = knownMap;
    return robot;
}

int moveFwd(Map *map, Robot *robot, int dist)
{
    MapPoint *target = getTarget(robot);
    int x = target->x;
    int y = target->y;//Lisibilité pour la suite

    //Jette un caillou jaune sur le chemin
    MoveCursorTo(robot->x, robot->y);
    setTerminalColor(CL_RESET);

    if ( robot->angle )
    {
        setTerminalColor(CL_YELLOW);
    } else {
        setTerminalColor(CL_BLUE);
    }
    printf("×");    //Ceci n'est pas un mur mais un caillou dans le terminal, c'est le vrai signe de la multiplication (×) et pas un x

    robot->knownMap[robot->y][robot->x] ='.';  //!\ ceci est le caillou dans le tableau du robot

    //Vérification de la possibilité du déplacement demandé (ie. qu'il n'y a pas de mur et qu'on ne va pas a plus de 4 cases d'un coup)
    if ( (map->map[y][x] != 'x') && ( (abs(robot->x - x) < 4) && (abs(robot->y - y) < 4) ) )
    {
        //Déplacer le robot
        robot->x = x;
        robot->y = y;
        robot->steps += dist;

        //Mise à jour de la map du robot avec sa propre position
        robot->knownMap[robot->y][robot->x] = 'R';

        //Afficher le robot (terminal)
        MoveCursorTo(robot->x, robot->y);
        setTerminalColor(BK_RED);
        printf(" ");
        fflush(stdout);

        return 1;
    } else {
        robot->knownMap[y][x] = map->map[y][x]; //Place ce mur dans la map connue du robot
        robot->lockDir = 0;                     //Déverrouille la direction si elle était verrouillée

        MoveCursorTo(robot->x, robot->y);
        setTerminalColor(BK_RED);
        printf(" ");
        fflush(stdout);

        return 0;
    }
}

void turn(Robot *robot, int dir, char mode)
{
    if ( !robot->lockDir )
    {
        if( mode )//Absolu ou relatif
        {   //Relatif
            if ( dir==LEFT )
            {
                robot->dir -= 1;
                robot->dir = (robot->dir < 0)? 3 : robot->dir;
                robot->angle--;
            }
            else if (dir==RIGHT)
            {
                robot->dir = (robot->dir +1 )%4;
                robot->angle++;
            }
        } else {//Absolu
            robot->dir = dir;
        }
    }
}

void reachWall(Map *map, Robot *robot)
{
    //Recherche d'un mur
    robot->lockDir = 1;
    if ( !moveFwd(map, robot, 1) )
    {
        robot->lockDir=0;//Déverouiller la directionquand un mur est atteint
    }
}

int Bot_FollowWall(GUI_Component *window, Map *map, Robot *robot)//Robot stupide de type peu intelligent. Assez collant, surtout sur les murs.
{
    static int ratioRandom = 0;

    MapPoint *target = getTarget(robot);
    char trgt = getKnownCase(map->map, target->x, target->y, map->width, map->height);

    RestoreCursorPosition();
    printf("\nDir=%d (lock:%d), angle=%d, seeking (%c), pos=(%d,%d), seek=(%d,%d)",robot->dir,robot->lockDir, robot->angle, trgt , robot->x, robot->y, target->x, target->y);


    if ( checkArrival(map, robot) ) return 0;       //Gagné !

    if ( !moveFwd(map, robot, 1) )                  //S'il y a un mur devant
    {
        turn(robot, RIGHT,MOVE_RELATIVE);           //Dans ce cas on tourne à droite...

        if ( !moveFwd(map, robot, 1) )              //et on avance
        {
            turn(robot, RIGHT, MOVE_RELATIVE);      //... ou pas, dans ce cas, encore à droite
        }
    }
    else if ( robot->angle == 0 )                   //Si le compteur de Pledge arrive à 0, on continue tout droit
    {
        turn(robot, RIGHT, MOVE_RELATIVE);
    }

    target = getTarget(robot);
    trgt = getKnownCase(robot->knownMap, target->x, target->y, map->width, map->height);    //on regarde si on est déja allé sur la case ou l'on se prépare à aller

    turn(robot, LEFT, MOVE_RELATIVE);               //On se présente face au mur (si mur il y a)

    if ( (trgt == '.') && (countNearestUnvisitedCases(robot) > 0 && countNearestUnvisitedCases(robot) < 4) )    //S'il reste *certaines* cases vides, il est possible de lancer l'exploration récursive
    {
        RestoreCursorPosition();
        printf("Recursive Pathfinder @(%d,%d) w. %d open cases\n", robot->x, robot->y, countNearestUnvisitedCases(robot));

        ScanMap(window, map, robot, LEFT);
    }

    setTerminalColor(CL_RESET);
    fflush(stdout);

    return 1;
}


int ScanMap(GUI_Component *window, Map *map, Robot *robot, int dir)
{
    if ( checkArrival(map, robot) ) return 1;           //Si on est à l'arrivée, inutile d'aller faire des segfaults

    turn(robot, dir, MOVE_ABSOLUTE);                    //On tourne le robot dans la direction choisie

    MapPoint *target = getTarget(robot);

    if ( robot->knownMap[target->y][target->x] != '.' ) //Vérifier si on est pas déja allé à ce point
    {
        if ( moveFwd(map, robot, 1) )                   //Si on peut y avancer...
        {
            //Affichage du robot
            DrawMap(window, map, robot);
            RenderGUI(window);

            //Gestion des événements
            ManageEvents(window);

            wait(DELAY/4);

            //Exploration des cases adjacentes (4 cardiaux >> rotations absolues)
            if ( ScanMap(window, map, robot, LEFT) ) return 1;
            if ( ScanMap(window, map, robot, UP) ) return 1;
            if ( ScanMap(window, map, robot, RIGHT) ) return 1;
            if ( ScanMap(window, map, robot, DOWN) ) return 1;

            //Retour au sens dans lequel il est arrivé sur cette case
            turn(robot, dir, MOVE_ABSOLUTE);

            //Demi tour
            turn(robot, RIGHT, MOVE_RELATIVE);
            turn(robot, RIGHT, MOVE_RELATIVE);

            //Avancer sur la case d'avant
            moveFwd(map, robot, 1);
        }

        //Affichage du robot
        DrawMap(window, map, robot);
        RenderGUI(window);

        //Gestion des événements
        ManageEvents(window);
        wait(DELAY/4);
    }
    return 0;
}

MapPoint *getTarget(Robot *robot)
{
    MapPoint *target = malloc(sizeof(MapPoint));

    switch ( robot->dir ) {
        case UP:
            target->y = robot->y - 1;
            target->x = robot->x;
            break;
        case DOWN:
            target->y = robot->y + 1;
            target->x = robot->x;
            break;
        case LEFT:
            target->y = robot->y;
            target->x = robot->x - 1;
            break;
        case RIGHT:
            target->y = robot->y;
            target->x = robot->x + 1;
            break;
    }

    return target;
}

char getKnownCase(char **map, int x, int y, int width, int height)
{
    if ((x >= 0 && x < width) && (y >= 0 && y < height))
    {
        return map[y][x];
    } else {
        return '\0';
    }
}

int countNearestUnvisitedCases(Robot *robot)
{
    return (robot->knownMap[robot->y][robot->x+1] == ' ') + (robot->knownMap[robot->y][robot->x-1] == ' ') + (robot->knownMap[robot->y-1][robot->x] == ' ') + (robot->knownMap[robot->y+1][robot->x] == ' ');
}

int checkArrival(Map *map, Robot *robot)
{
    return ( (abs(robot->x - map->arrival.x) + abs(robot->y - map->arrival.y))  < 2 );
}

void displayRobotMap(Map *map, Robot *robot)
{

    for ( int y=0 ; y < map->height ; y++ )
    {
        for ( int x = 0 ; x < map->width ; x++ )
        {
            printf("%c",robot->knownMap[y][x]);
        }
        setTerminalColor(CL_RESET);
        printf("\n");
    }
}

void FreeRobot(Robot *robot, int mapSize)
{
    sfrMap(robot->knownMap, mapSize);
    free(robot);
}
