
#include "ai.h"
#include "map.h"
#include "func.h"
#include "interface.h"

int main(int argc, char const *argv[])
{

    /*Chargement des ressources*/

    Map *map = NULL;

    //Chargement de la map (ou map custom si précisé)
    if (argc > 1)
    {
        map = LoadMap(argv[1]);
    } else {
        map = LoadMap("maps/appart.txt");
    }


    system("clear");

    PrintMap(map);
    printf("Départ : (%d,%d)\nArrivée : (%d,%d)", map->start.x, map->start.y, map->arrival.x, map->arrival.y);

    SaveCursorPosition();

    Robot *robot = createRobot(map);

    GUI_Component *window = initGUI(map,MODE_ISOMETRIC);

    robot->lockDir = 1;

    /* Pathfinding et gestion graphique */
    DrawMap(window, map, robot);
    RenderGUI(window);

    do
    {
        //Effacer le robot
        //ClearRobot(window, robot);

        //Exécution d'une étape
        Bot_FollowWall(window, map, robot);

        //Affichage du robot
        //DrawRobot(window, robot);
        DrawMap(window, map, robot);
        RenderGUI(window);

        RestoreCursorPosition();
        printf("\rKilométrage : %d\n", robot->steps);

        wait(DELAY);

    } while ( ManageEvents(window) );

    //Trucs peu utiles mais jolis
    // setTerminalColor(CL_RED);
    // printf("FINI!!!!!\n\n\n\n");
    // setTerminalColor(CL_RESET);

    RestoreCursorPosition();
    printf("\n");

    FreeRobot(robot, map->height);
    FreeMap(map);
    FreeGUI(window);

    return 0;
}
