
#include "ai.h"
#include "map.h"
#include "func.h"
#include "interface.h"

int main(int argc, char const *argv[])
{

    /*Chargement des ressources*/

    Map *map = NULL;
    int mode = MODE_ISOMETRIC;
    int notFound = 1;

    //Chargement de la map (ou map custom si précisé)
    if (argc > 1)
    {
        map = LoadMap(argv[1]);
    } else {
        map = LoadMap("maps/appart.txt");
    }
    //Sélection du mode
    if (argc > 2 && !strcmp(argv[2], "2d"))
    {
        mode = MODE_2D;
    }

    //Affichage terminal
    system("clear");

    PrintMap(map);
    printf("Départ : (%d,%d), arrivée : (%d,%d)\n", map->start.x, map->start.y, map->arrival.x, map->arrival.y);

    SaveCursorPosition();

    //Création du robot et de l'interface
    Robot *mrHandy = createRobot(map);

    GUI_Component *window = initGUI(map,mode);

    mrHandy->lockDir = 1;

    //Affichage de la map
    DrawMap(window, map, mrHandy);
    RenderGUI(window);

    do
    {
        //Exécution d'une étape
        if ( notFound ) notFound = Bot_FollowWall(window, map, mrHandy);

        //Affichage du mrHandy et de la map
        DrawMap(window, map, mrHandy);
        if ( !notFound )
        {
            char msg[20];
            sprintf(msg, "Path found in %d moves !", mrHandy->steps);
            DrawText(window, window->font, 10, 10, msg, TXT_DEFAULT);
        }

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
