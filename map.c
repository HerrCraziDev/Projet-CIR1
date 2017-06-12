#include "map.h"
#include "func.h"

Map *LoadMap(const char *file)
{
    Map *out = malloc(sizeof(Map));

    char *line = malloc(sizeof(char) * 8);
    int width, height, i, j;

    FILE *fp = fopen(file,"r");


    //Vérification fichier
    if (!fp)
    {
        printf("Erreur à l'ouverture du fichier de map %s\n", file);
        return NULL;
    }


    //Détermination de la hauteur et la largeur
    fgets(line, 8, fp);
    width = atoi(strfield(line,':',0));
    height = atoi(strfield(line,':',1));

    printf("Taille : %d×%d\n", width, height);


    //Allocation de la map
    char **map = malloc(sizeof(char *) * height);

    for (i = 0 ; i < height ; i++)
        map[i] = malloc(sizeof(char) * (width+1));


    //Remplissage depuis le fichier
    line = realloc(line, sizeof(char) * (width+1));

    for (i=0 ; i < height ; i++)
    {
        fgets(line, width+2, fp);

        for (j=0 ; j < width ; j++)
        {
            map[i][j] = line[j];

            if (line[j] == 'D')
            {
                out->start.x = j;
                out->start.y = i;
            } else if (line[j] == 'S'){
                out->arrival.x = j;
                out->arrival.y = i;
            }
        }
    }

    //Vérification de l'intégrité de la map (il ne doit pas rester de données non lues)
    if ( fgets(line, i++, fp) )
    {
        setTerminalColor(CL_RED);
        printf("Erreur : La map semble corrompue ou inexacte. Les informations de taille peuvent être erronnées. Utiliser cette map risque de provoquer un segfault méchant.\nContinuer quand même (vous êtes prévenu) ? y/(n) : ");
        readString(line,3);

        if(line[0]!='y')
        {
            exit(EXIT_FAILURE);
        }
    }

    //Fermeture du fichier
    fclose(fp);
    
    out->width = width;
    out->height = height;
    out->map = map;
    return out;
}

void PrintMap(Map *map)
{
    for (int y=0 ; y < map->height ; y++)
    {
        for (int x = 0 ; x < map->width ; x++)
        {
            switch (map->map[y][x])
            {
                case 'x':
                    setTerminalColor(BK_WHITE);
                    setTerminalColor(CL_BLACK);
                    printf(" ");
                    break;
                case ' ':
                    setTerminalColor(CL_RESET);
                    printf(" ");
                    break;
                case 'D':
                    setTerminalColor(CL_WHITE);
                    setTerminalColor(BK_BLUE);
                    printf("D");
                    break;
                case 'S':
                    setTerminalColor(CL_RED);
                    setTerminalColor(BK_GREEN);
                    printf("S");
                    break;
            }
        }
        setTerminalColor(CL_RESET);
        printf("\n");
    }
}

void sfrMap(char **map, int size)
{
    for (int i = 0 ; i < size ; i++)
    {
        free(map[i]);
    }
    free(map);
}

void FreeMap(Map *map)
{
    sfrMap(map->map, map->height);
    free(map);
}
