pi: 
	gcc main.c -w -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread -lm -O2 -DRPI -o prog
	./prog
npi: 
	gcc main.c -w -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread -lm -O2 -o prog
	./prog
