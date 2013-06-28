execName = ep3
testeName = teste
execOptions =  -g -o $(execName) -lm -lc -lallegro -lallegro_primitives -lallegro_dialog -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec
testeOptions = -g -o $(testeName) -lm -lc -lallegro -lallegro_primitives -lallegro_dialog -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec
objOptions = -g -pedantic -Wall -lm -lc -lallegro -lallegro_dialog -lallegro_primitives -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec
objectsAllegro = main.o PrintAllegro.o grade.o rio.o util.o pixel.o Vector_2D.o
objectsTeste = teste.o PrintAllegro.o grade.o rio.o util.o pixel.o rotinasTeste.o


.PHONY:allegro
allegro: $(objectsAllegro)
	gcc $(objectsAllegro) $(execOptions)

.PHONY:teste
teste: $(objectsTeste)
	gcc $(objectsTeste) $(testeOptions)

teste.o: teste.c
	gcc -c teste.c $(objOptions)

main.o: main.c
	gcc -c main.c $(objOptions) 

PrintAllegro.o: PrintAllegro.c
	gcc -c PrintAllegro.c $(objOptions)

grade.o: grade.c
	gcc -c grade.c $(objOptions)

rio.o: rio.c
	gcc -c rio.c $(objOptions)

util.o: util.c
	gcc -c util.c $(objOptions)

pixel.o: pixel.c
	gcc -c pixel.c $(objOptions)

Vector_2D.o:
	gcc -c Vector_2D.c $(objOptions)

config.yy.o: config.yy.c config.tab.c
	gcc -c config.yy.c

config.tab.o: config.tab.c
	gcc -c config.tab.c

config.yy.c: config.l config.tab.h
	flex -o config.yy.c config.l

config.tab.c config.tab.h: config.y
	bison -v -t --defines=config.tab.h -o config.tab.c config.y


rotinasTeste.o: rotinasTeste.c
	gcc -c rotinasTeste.c $(objOptions)

clean:
	rm -f *.o *.yy.c *.tab.c $(execName)
