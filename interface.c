#include "interface.h"

GUI_Component *initGUI(Map *map)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("Erreur d'initialisation du systéme graphique SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *hWnd = OpenWindow(map->width*GUI_TILESIZE, map->height*GUI_TILESIZE, "Pathfinder Mk1.0", SDL_HWSURFACE | SDL_DOUBLEBUF);

    srand(time(NULL));

    SDL_FillRect(hWnd, NULL, 0xffffffff);

    GUI_Component *window = malloc(sizeof(GUI_Component));
    window->internal = hWnd;
    window->width = map->width*GUI_TILESIZE;
    window->height =  map->height*GUI_TILESIZE;

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
    DrawTile(window->internal, robot->x, robot->y, 'R');
}

void ClearRobot(GUI_Component *window, Robot *robot)
{
    DrawTile(window->internal, robot->x, robot->y, '.');
}

void DrawTile(SDL_Surface *output, int x, int y, char type)
{
    SDL_Surface *tile = SDL_CreateRGBSurface(0, GUI_TILESIZE, GUI_TILESIZE, 32, 0, 0, 0, 0);

    SDL_Rect rect_dest;
    rect_dest.x = x*GUI_TILESIZE;
    rect_dest.y = y*GUI_TILESIZE;

    switch (type) {
        case 'x':
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 60, 60, 50));
        break;
        case 'R':
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 255, 0, 0));
        break;
        case 'D':
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 0, 0, 200));
        break;
        case 'S':
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 0, 200, 0));
        break;
        case '.':
        SDL_FillRect(tile, NULL, SDL_MapRGB(tile->format, 255, 240, 220));
        break;
    }

    SDL_BlitSurface(tile, NULL, output, &rect_dest);

    SDL_FreeSurface(tile);
}

void DrawMap(GUI_Component *output, Map *map)
{
    for (int y=0 ; y < map->height ; y++)
    {
        for (int x = 0 ; x < map->width ; x++)
        {
            DrawTile(output->internal, x, y, map->map[y][x]);
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

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            return 0;

            case SDL_MOUSEBUTTONDOWN:
            debugMode = !debugMode;
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
