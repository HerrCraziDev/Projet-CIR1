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
        //Calcul de la hauteur de la fenêtre et de la largeur (identiques, c'est un carré)
        window->width = (map->width/2 + map->height/2) * GUI_TILESIZE +10;
        window->height = window->width + GUI_TILESIZE +10;
        window->outputOffset = (map->width/2) * GUI_TILESIZE;

        //Chargement des textures
        window->textures[TX_WALL] = SDL_LoadBMP("ressources/16x16/wall.bmp");
        window->textures[TX_ROBOT] = SDL_LoadBMP("ressources/16x16/robot.bmp");
        window->textures[TX_GROUND] = SDL_LoadBMP("ressources/16x16/grass.bmp");
        window->textures[TX_START] = SDL_LoadBMP("ressources/16x16/start.bmp");
        window->textures[TX_ARRIVAL] = SDL_LoadBMP("ressources/16x16/arrival.bmp");
        window->textures[TX_VISITED] = SDL_LoadBMP("ressources/16x16/dry_grass.bmp");

        for (int i = 0 ; i < GUI_NB_TEXTURES ; i++)
        {
            //Couleur de transparence sur les textures (noir)
            SDL_SetColorKey(window->textures[i], SDL_SRCCOLORKEY, SDL_MapRGB(window->textures[i]->format, 0, 0, 0));
        }
    }
    else
    {
        window->width = map->width*GUI_TILESIZE;
        window->height =  map->height*GUI_TILESIZE;
    }


    //Création d'une fenêtre
    SDL_Surface *hWnd = OpenWindow(window->width, window->height, "Robo-Labour-O-matic' 2000 Mk1.0", SDL_HWSURFACE | SDL_DOUBLEBUF);

    //Couleur d'arriére plan
    SDL_FillRect(hWnd, NULL, 0x11111100);

    window->internal = hWnd;


    //Initialisation du générateur pseudo aléatoire
    srand(time(NULL));


    //Chargement d'une police et affichage
    TTF_Init();
    window->font = TTF_OpenFont("ressources/terminal.ttf", 25);    //Fonte par défaut du jeu
    TTF_Font *title = TTF_OpenFont("ressources/minecraft.ttf", 42); //spécial titre

    DrawText(window, title, window->width/2, window->height/2, "Robo-Labour-O-matic 2000", TXT_CENTERED);
    DrawText(window, window->font, window->width/2, (window->height/2) +50, "Loading...", TXT_CENTERED);

    TTF_CloseFont(title);
    SDL_Flip(window->internal);


    //Initialisation de l'environnement audio
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_AllocateChannels(2);

    window->music = Mix_LoadWAV("ressources/soundtrack.ogg");

    Mix_PlayChannel(-1, window->music, 0);
    return window;
}


SDL_Surface *OpenWindow(int width,int height, char *title, int flags)
{
    SDL_Surface *hWnd = SDL_SetVideoMode(width,height,32,flags);
    SDL_WM_SetCaption(title,NULL);
    return hWnd;
}

void DrawText(GUI_Component *window, TTF_Font *font, int x, int y, char *message, int mode)
{
    SDL_Color color = {255,255,255};

    SDL_Surface *text = TTF_RenderText_Blended(font, message, color);

    SDL_Rect dest;
    if (mode==TXT_DEFAULT)
    {
        dest.x = x;
        dest.y = y;
    } else {
        dest.x = x - text->w/2;
        dest.y = y - text->h/2;
    }

    //Affichage du message de chargement
    SDL_BlitSurface(text, NULL, window->internal, &dest);

    SDL_FreeSurface(text);
}

void DrawRobot(GUI_Component *window, Robot *robot)
{
    if (window->mode)
    {
        DrawIsometricTile(window, robot->x, robot->y, 'R');
    } else {
        DrawTile(window, robot->x, robot->y, 'R');
    }
}

void ClearRobot(GUI_Component *window, Robot *robot)
{
    if (window->mode)
    {
        DrawIsometricTile(window, robot->x, robot->y, '.');
    } else {
        DrawTile(window, robot->x, robot->y, '.');
    }
}

void DrawIsometricTile(GUI_Component *output, int x, int y, char type)
{
    SDL_Surface *tile = NULL;

    //Transformation des coordonnées cartésiennes en coordonnées isométriques
    SDL_Rect rect_dest;
    rect_dest.x = (x*GUI_TILESIZE)/2 + (y*GUI_TILESIZE)/2;
    rect_dest.y = (y*GUI_TILESIZE)/2 - (x*GUI_TILESIZE)/2 + output->outputOffset;

    //Sélection de la texture adaptée
    switch (type)
    {
        case 'x':
        rect_dest.y -= TX_WALL_YOFFSET;
        tile = output->textures[TX_WALL];
        break;
        case 'D':
        tile = output->textures[TX_START];
        break;
        case 'S':
        tile = output->textures[TX_ARRIVAL];
        break;
        case ' ':
        tile = output->textures[TX_GROUND];
        break;
        case 'R':
        tile = output->textures[TX_ROBOT];
        break;
        case '.':
        tile = output->textures[TX_VISITED];
        break;
    }
    //Blit de la surface sur la sortie graphique (fenêtre)
    SDL_BlitSurface(tile, NULL, output->internal, &rect_dest);
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

void DrawMap(GUI_Component *output, Map *map, Robot *robot)
{
    //Effacement
    SDL_FillRect(output->internal, NULL, 0x0);

    int x, y, x_dep, y_dep;

    //Affichage des tiles en commençant en haut à droite, puis jusqu'à la diagonale en biais
    for ( x_dep = map->width ; x_dep >= 0 ; x_dep--)
    {
        for ( x = x_dep, y = 0 ; (x<map->width && y<map->height) ; x++, y++)
        {
            if (output->mode)
            {
                if( (map->map[y][x] != 'D') && (robot->knownMap[y][x] == '.' || robot->knownMap[y][x] == 'R') )
                {
                    DrawIsometricTile(output, x, y, robot->knownMap[y][x]);
                } else {
                    DrawIsometricTile(output, x, y, map->map[y][x]);
                }
            } else {
                DrawTile(output, x, y, map->map[y][x]);
                DrawTile(output, robot->x, robot->y, 'R');
            }
        }
    }
    //Affichage de la diagonale au coin inférieur gauche
    for ( y_dep = 0 ; y_dep < map->height ; y_dep++)
    {
        for ( x = 0, y = y_dep ; (x<map->width && y<map->height) ; x++, y++)
        {
            if (output->mode)
            {
                if( (map->map[y][x] != 'D') && (robot->knownMap[y][x] == '.' || robot->knownMap[y][x] == 'R') )
                {
                    DrawIsometricTile(output, x, y, robot->knownMap[y][x]);
                } else {
                    DrawIsometricTile(output, x, y, map->map[y][x]);
                }
            } else {
                DrawTile(output, x, y, map->map[y][x]);
                DrawTile(output, robot->x, robot->y, 'R');
            }
        }
    }
}

void RenderGUI(GUI_Component *output)
{
    SDL_Flip(output->internal);
}


int ManageEvents(GUI_Component *window)
{
    static int pause = 0;

    SDL_Event event;

    //Boucle de pause
    do {

        //Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT://Quitter
                return 0;

                case SDL_MOUSEBUTTONDOWN://Pause
                pause = !pause;
                break;
            }
        }

        if (pause) wait(DELAY); //Ne pas consommer trop de CPU en pause

    } while (pause);

    return 1;
}

void FreeGUI(GUI_Component *window)
{
    //Libération des textures
    for (int i = 0 ; i < GUI_NB_TEXTURES ; i++)
    {
        SDL_FreeSurface(window->textures[i]);
    }

    //Libération de la musique et des canaux virtuels
    Mix_FreeChunk(window->music);
    Mix_CloseAudio();

    //Fermeture du systéme d'affichage texte
    TTF_CloseFont(window->font);
    TTF_Quit();

    //Destruction de l'environnement graphique
    SDL_Quit();

    free(window);
}
