#include "func.h"

//Sépare une chaine en sous chaines délimitées par un séparateur et récupére la sous chaine d'indice <field>
char *strfield(char *string, char separator, int field)
{
    char *ret = calloc(strlen(string),sizeof(char));
    strcpy(ret,string);
    int len = strlen(ret);

    for (size_t i = 0; i < len ; i++)
    {
        if (ret[i] == separator)
        {
            ret[i] = '\0';
        }
    }

    char *ptr = ret;
    for (int sepCounter=0 ; sepCounter < field ; ptr++)
    {
        if (! *ptr) sepCounter++;
        //printf("%c", (char)*ptr);
    }

    return ptr;
}

void setTerminalColor(char *param) //Change la couleur du texte dans le terminal C'est zoli :)
{
	printf("\033[%sm",param);
}

void wait(unsigned int millis)
{
    const clock_t start = clock();

    clock_t current;
    do {
        current = clock();
    } while((unsigned int)(current-start)/(CLOCKS_PER_SEC/1000) < millis);

}

int waitInput() //Attend qu'une touche soit pressée
{
    char key = 0;
    while (key==0)
    {
        key = getchar();
    }
    return key;
}

char *readString(char *Out,int MaxLen) //Attend la saisie d'une ligne et retire l'\n de fin
{
	fgets(Out,MaxLen,stdin);
	Out[strlen(Out)-1] = '\0';
	return Out;
}

int randInt(int minVal, int maxVal)
{
    return (rand() % (maxVal+1 - minVal)) + minVal;
}
