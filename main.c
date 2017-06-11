
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

    Robot *mrHandy = createRobot(map);

    GUI_Component *window = initGUI(map,MODE_ISOMETRIC);

    mrHandy->lockDir = 1;

    /* Pathfinding et gestion graphique */
    DrawMap(window, map, mrHandy);
    RenderGUI(window);

    do
    {
        //Exécution d'une étape
        Bot_FollowWall(window, map, mrHandy);

        //Affichage du mrHandy et de la map
        DrawMap(window, map, mrHandy);
        RenderGUI(window);

        RestoreCursorPosition();
        printf("\rKilométrage : %d\n", mrHandy->steps);

        wait(DELAY);

    } while ( ManageEvents(window) );

    RestoreCursorPosition();
    printf("\n");

    //Libération des ressources
    FreeRobot(mrHandy, map->height);
    FreeMap(map);
    FreeGUI(window);

    return 0;
}
