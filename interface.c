#include "interface.h"

GUI_Component *initGUI(Map *map, int mode)
{
    //Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("Erreur d'initialisation du systéme graphique SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Initialisation de la struct GUI_Component
    GUI_Component *window = malloc(sizeof(GUI_Component));
    window->mode = mode;

    if (mode == MODE_ISOMETRIC)
    {
        window->width = (map->width/2 + map->height/2) * GUI_TILESIZE;
        window->height = window->width;
        window->outputOffset = (map->width/2) * GUI_TILESIZE;
        //printf("Width : %d, height : %d\n", window->wid);
    } else {
        window->width = map->width*GUI_TILESIZE;
        window->height =  map->height*GUI_TILESIZE;
    }

    //Création d'une fenêtre
    SDL_Surface *hWnd = OpenWindow(window->width, window->height, "Pathfinder Mk1.0", SDL_HWSURFACE | SDL_DOUBLEBUF);

    //Initialisation du générateur pseudo aléatoire
    srand(time(NULL));

    //Couleur d'arriére plan
    SDL_FillRect(hWnd, NULL, 0xffffffff);

    window->internal = hWnd;


    return window;
}


SDL_Surface *OpenWindow(int width,int height, char *title, int flags)
{
    SDL_Surface *hWnd = SDL_SetVideoMode(width,height,32,flags);
    SDL_WM_SetCaption(title,NULL);
    return hWnd;
}

void DrawRobot(GUI_Component *window, Robot *robot)
{
    DrawTile(window, robot->x, robot->y, 'R');
}

void ClearRobot(GUI_Component *window, Robot *robot)
{
    DrawTile(window, robot->x, robot->y, '.');
}

void DrawTile(GUI_Component *output, int x, int y, char type)
{
    //Création d'une surface temporaire pour l'affichage d'une tuile
    SDL_Surface *tile = SDL_CreateRGBSurface(0, GUI_TILESIZE, GUI_TILESIZE, 32, 0, 0, 0, 0);

    //Destination
    SDL_Rect rect_dest;

    if ( output->mode )
    {
        rect_dest.x = (x*GUI_TILESIZE)/2 + (y*GUI_TILESIZE)/2;
        rect_dest.y = (y*GUI_TILESIZE)/2 - (x*GUI_TILESIZE)/2 + output->outputOffset;
    } else {
        rect_dest.x = x*GUI_TILESIZE;
        rect_dest.y = y*GUI_TILESIZE;
    }

    //Couleur de la surface
    switch (type) {
        case 'x'://Mur
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 60, 60, 50));
        break;
        case 'R'://Robot
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 255, 0, 0));
        break;
        case 'D'://Départ
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 0, 0, 200));
        break;
        case 'S'://Arrivée
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 0, 200, 0));
        break;
        case '.'://Case visitée
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 220, 200, 180));
        break;
        case ' '://Case non visitée
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 255, 255, 255));
        break;
    }
    //Blit de la surface sur la sortie graphique (fenêtre)
    SDL_BlitSurface(tile, NULL, output->internal, &rect_dest);
    //Destruction de la surface
    SDL_FreeSurface(tile);
}

void DrawMap(GUI_Component *output, Map *map)
{
    for (int y=0 ; y < map->height ; y++)
    {
        for (int x = 0 ; x < map->width ; x++)
        {
            DrawTile(output, x, y, map->map[y][x]);
        }
    }
}

void RenderGUI(GUI_Component *output)
{
    SDL_Flip(output->internal);
}


int ManageEvents(GUI_Component *window)
{
    static int debugMode = 0;

    SDL_Event event;

    //Gestion des événements
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            return 0;

            case SDL_MOUSEBUTTONDOWN://Toggle du mode debug (n.imp.)
            debugMode = !debugMode;

            wait(1000);
            break;
        }
    }


    return 1;
}

void FreeGUI(GUI_Component *window)
{
    // free(window->internal);
    // free(window);
    SDL_Quit();
}
