pi: 
	gcc main.c -w -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread -lm -DRPI -o prog
	./prog
npi: 
	gcc main.c -w -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread -lm -o prog
	./prog
