CFLAGS= -g
LFLAGS= -lSDL -lSDL_mixer -lSDL_ttf

robot.app: main.o ai.o interface.o map.o func.o
	gcc main.o ai.o func.o map.o interface.o $(LFLAGS) -o robot.app

ai.o: ai.c ai.h map.h interface.h func.h
	gcc -c $(CFLAGS) ai.c

map.o: map.c map.h func.h
	gcc -c $(CFLAGS) map.c

func.o: func.c func.h
	gcc -c $(CFLAGS) func.c

interface.o: interface.c interface.h func.h
	gcc -c $(CFLAGS) interface.c

main.o: main.c ai.h map.h interface.h func.h
	gcc -c $(CFLAGS) main.c

undev:
	rm *.o

clean:
	rm *.o *.app
