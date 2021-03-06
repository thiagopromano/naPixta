

#include "common.c"
#include "sprite.c"
#define X_SETAS_INICIAL 20
#define Y_SETAS_INICIAL 50
int X_SETAS_DISTANCIA = 85;
#define Y_SETAS_DISTANCIA 80

SDL_Texture *text_texture1 = NULL;
TTF_Font *font;
int simulated[9] = {0};

int score = 0;
SDL_Texture *setasTexture[18];
Animation *setasAnimation[18];
int selecionado = 0;


struct musica atual;

void showSplashScreen(void)
{

	SDL_Rect rect;
	SDL_Texture *splashTexture = getTexture("assets/pixta.jpg");
	int w, h;
	SDL_QueryTexture(splashTexture, NULL, NULL, &w, &h);
	rect.w = viewport.w;
	rect.h = viewport.h;
	rect.x = 0;
	rect.y = 0;

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);

	SDL_Delay(100);
	int alpha = 1;
	while (alpha < 255)
	{
		if (SDL_SetTextureAlphaMod(splashTexture, alpha) != 0)
		{
			quit(1);
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, splashTexture, NULL, &rect);
		SDL_RenderPresent(renderer);

		SDL_Delay(TICK_INTERVAL);
		alpha = alpha + 6;
	}
}

void playSample(int sel)
{
	system("killall mpg123");
	char buf[150];
	sprintf(buf, "mpg123 -k 1000 %s &", musicas[sel].musica);
	system(buf);
}

int showMenu(void)
{
	SDL_Event event;
	SDL_Rect rect;

	int w, h;

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);

	SDL_Delay(100);
	int alpha = 1;

	int done = 0;
	SDL_Texture *setaMenu = getTexture("assets/Setas/Normal_Left.png");
	SDL_Texture *setaMenu2 = getTexture("assets/Setas/Normal_Right.png");

	SDL_QueryTexture(setaMenu, NULL, NULL, &w, &h);

	int ySeta = viewport.h / 2 - h / 2;
	playSample(selecionado);
	while (!done)
	{

		SDL_RenderClear(renderer);
		rect.y = ySeta;
		rect.x = 100 + (viewport.w - 200) / 6 - w / 2;
		rect.w = w;
		rect.h = h;
		SDL_RenderCopy(renderer, setaMenu, NULL, &rect);

		rect.x = 100 + 5 * (viewport.w - 200) / 6 - w / 2;

		SDL_RenderCopy(renderer, setaMenu2, NULL, &rect);

		if (text_texture1)
		{
			SDL_DestroyTexture(text_texture1);
		}

		char buffer[50];

		sprintf(buffer, "%s", musicas[selecionado].nome);

		text_texture1 = renderFontToTexture(font, buffer);

		SDL_Rect text_rect;
		int wtext, htext;
		SDL_QueryTexture(text_texture1, NULL, NULL, &wtext, &htext);
		text_rect.x = viewport.w / 2 - wtext / 2;
		text_rect.y = viewport.h / 2 - htext / 2;
		SDL_QueryTexture(text_texture1, NULL, NULL, &text_rect.w,
				 &text_rect.h);
		SDL_RenderCopy(renderer, text_texture1, NULL, &text_rect);

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event))
		{
			// printf("Event type: %d\n",
			// event.type);
			if (event.type == SDL_QUIT ||
			    event.type == SDL_WINDOWEVENT_CLOSE)
			{
				done = 2;
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 2;
				if (event.key.keysym.sym - SDLK_KP_1 >= 0 &&
				    event.key.keysym.sym - SDLK_KP_1 < 9)
				{
					int seta =
					    event.key.keysym.sym - SDLK_KP_1;
					if (simulated[seta] == 0)
					{
						if (seta == 5)
						{
							selecionado =
							    (selecionado + 1) %
							    QTD_MUSICAS;
							playSample(selecionado);
						}
						else if (seta == 3)
						{
							selecionado =
							    (selecionado +
							     QTD_MUSICAS - 1) %
							    QTD_MUSICAS;
							playSample(selecionado);
						}
						else if (seta == 4)
						{
							done = 1;
						}

						printf("selecionado = %d\n",
						       selecionado);
						printf("up %d\n", seta);
					}
				}
			}
		}
	}
	system("killall mpg123");
	atual = musicas[selecionado];
	return done;
}

void handleButton(int seta, int down_or_up, long int tempoMusica)
{
	// down
	if (down_or_up == 1)
	{
		float tempoPorPasso = atual.tempoPorPasso;

		float passoAtual = (tempoMusica / 1000.0f) / tempoPorPasso;

		int iPasso = round(passoAtual);
		float diferenca = fabs(passoAtual - iPasso);
		if (diferenca < 0.3 &&
		    atual.step[iPasso][seta] != 0)
		{
			atual.step[iPasso][seta] = 0;

			if (diferenca < 0.1)
				score += 200;
			else if (diferenca < 0.2)
				score += 100;
			else if (diferenca < 0.3)
				score += 50;
		}
		printf("passoAtual %f", passoAtual);
	}
	else // up
	{
	}

	// printf("Keyboard event wasd %d, %d, %d, %d\n", wasd[0], wasd[1],
	// wasd[2], wasd[3]);
}

void drawSetasTopo(int frameNum)
{
	int x = X_SETAS_INICIAL;
	int y = Y_SETAS_INICIAL;

	Sprite *seta;

	seta = getSpriteFromAnimation(setasAnimation[9], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[12], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[15], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[16], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[13], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[10], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[11], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[14], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[17], frameNum);
	drawSpriteAt(renderer, seta, x, y);
}

drawSetasGame(int frameNum, long int tempoMusica)
{

	// int x = X_SETAS_INICIAL;
	int y = Y_SETAS_INICIAL;
	float tempoPorPasso = atual.tempoPorPasso;

	float passoAtual = (tempoMusica / 1000.0f) / tempoPorPasso;
	// printf("TempoPorPasso %f\n", tempoPorPasso);
	// printf("estou no tempo %d\n", tempoMusica);
	// printf("estou no passo %f\n\n\n", passoAtual);
	int i;

	Sprite *setas[9];
	for (i = 0; i < 9; i++)
	{
		setas[i] = getSpriteFromAnimation(setasAnimation[i], frameNum);
	}

	for (i = MAX(0, passoAtual - 1); i < passoAtual + 20; i++)
	{
		int x = X_SETAS_INICIAL;
		if (atual.step[i][0])
		{
			drawSpriteAt(renderer, setas[0], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][3])
		{
			drawSpriteAt(renderer, setas[3], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][6])
		{
			drawSpriteAt(renderer, setas[6], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][7])
		{
			drawSpriteAt(renderer, setas[7], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][4])
		{
			drawSpriteAt(renderer, setas[4], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][1])
		{
			drawSpriteAt(renderer, setas[1], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][2])
		{
			drawSpriteAt(renderer, setas[2], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][5])
		{
			drawSpriteAt(renderer, setas[5], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (atual.step[i][8])
		{
			drawSpriteAt(renderer, setas[8], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}
	}
}

void drawScore()
{
	if (text_texture1)
	{
		SDL_DestroyTexture(text_texture1);
	}

	char buffer[50];

	sprintf(buffer, "Score: %d", score);

	text_texture1 = renderFontToTexture(font, buffer);

	SDL_Rect text_rect;
	text_rect.x = 15;
	text_rect.y = 10;
	SDL_QueryTexture(text_texture1, NULL, NULL, &text_rect.w, &text_rect.h);
	SDL_RenderCopy(renderer, text_texture1, NULL, &text_rect);
}

int game(void)
{
	
	SDL_Event event;
	// SDL_SetRenderDrawColor(renderer, 0x70, 0xc8, 0x40, 0xff);
	
	SDL_Rect rect;
	SDL_Texture *splashTexture = getTexture("assets/bg-pixta.jpg");
	int w, h;
	SDL_QueryTexture(splashTexture, NULL, NULL, &w, &h);
	rect.w = viewport.w;
	rect.h = viewport.h;
	rect.x = 0;
	rect.y = 0;
	

	int charx = viewport.w / 2;
	int chary = viewport.h / 2;

	char buf[150];
	sprintf(buf, "mpg123 %s &", atual.musica);
	system(buf);
	
	
	// number of the current frame
	int frameNum = SDL_GetTicks() / TICK_INTERVAL;
	// amount of rendered frames
	int renderedFrames = 0;

	// desired frames by second
	int framesBySecond = 1000 / TICK_INTERVAL;

	printf("Desired fps %d\n", framesBySecond);
	printf("Start the game loop\n");
	
	long int tempoInicial = SDL_GetTicks();
	
	int done = 0;
	score = 0;
	
	while (!done)
	{

		long int tempoMusica = SDL_GetTicks() - tempoInicial;
		
		if (tempoMusica/1000 > atual.duracao)
		{
			done = 1;
		}

		// Check for events
		while (SDL_PollEvent(&event))
		{
			// printf("Event type: %d\n",
			// event.type);
			if (event.type == SDL_QUIT ||
			    event.type == SDL_WINDOWEVENT_CLOSE)
			{
				done = 2;
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 2;
				if (event.key.keysym.sym - SDLK_KP_1 >= 0 &&
				    event.key.keysym.sym - SDLK_KP_1 < 9)
				{
					int seta =
					    event.key.keysym.sym - SDLK_KP_1;
					if (simulated[seta] == 0)
					{
						printf("up %d\n", seta);
						handleButton(seta, 1,
							     tempoMusica);
						simulated[seta] = 1;
					}
				}
			}

			if (event.type == SDL_KEYUP)
			{

				if (event.key.keysym.sym - SDLK_KP_1 >= 0 &&
				    event.key.keysym.sym - SDLK_KP_1 < 9)
				{

					int seta =
					    event.key.keysym.sym - SDLK_KP_1;

					printf("down %d\n", seta);
					simulated[seta] = 0;
					handleButton(seta, 0, tempoMusica);
				}
				// handleKeyboard(event.key.keysym.sym,
				// 0);
			}
			//	}
		}

		// ---- Graphic rendering

		// this is not free
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, splashTexture, NULL, &rect);
		drawSetasTopo(frameNum);
		drawSetasGame(frameNum, tempoMusica);

		drawScore();

		// Update the screen

		SDL_RenderPresent(renderer);

		// Cap to ~ 50 fps

		capFramerate();
		
		frameNum++;

	}
	if (done == 1)
		return score;
	else
		return -1;
}

void *graficos(void *t)
{

	int i, j, k, done;

	init();
	X_SETAS_DISTANCIA = (viewport.w - X_SETAS_INICIAL * 2) / 9;
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	font = getFont("assets/yoster.ttf", 26);

	setasTexture[0] = getTexture("assets/Setas/Normal_Dig_Left_Down.png");
	setasTexture[1] = getTexture("assets/Setas/Normal_Down.png");
	setasTexture[2] = getTexture("assets/Setas/Normal_Dig_Right_Down.png");
	setasTexture[3] = getTexture("assets/Setas/Normal_Left.png");
	setasTexture[4] = getTexture("assets/Setas/Normal_Middle.png");
	setasTexture[5] = getTexture("assets/Setas/Normal_Right.png");
	setasTexture[6] = getTexture("assets/Setas/Normal_Dig_Left_Up.png");
	setasTexture[7] = getTexture("assets/Setas/Normal_Up.png");
	setasTexture[8] = getTexture("assets/Setas/Normal_Dig_Right_Up.png");	
	
	 setasTexture[9] = getTexture("assets/Setas/Gray_Dig_Left_Down.png");
	setasTexture[10] = getTexture("assets/Setas/Gray_Down.png");
	setasTexture[11] = getTexture("assets/Setas/Gray_Dig_Right_Down.png");
	setasTexture[12] = getTexture("assets/Setas/Gray_Left.png");
	setasTexture[13] = getTexture("assets/Setas/Gray_Middle.png");
	setasTexture[14] = getTexture("assets/Setas/Gray_Right.png");
	setasTexture[15] = getTexture("assets/Setas/Gray_Dig_Left_Up.png");
	setasTexture[16] = getTexture("assets/Setas/Gray_Up.png");
	setasTexture[17] = getTexture("assets/Setas/Gray_Dig_Right_Up.png");
	

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 64;
	rect.h = 64;

	for (i = 0; i < 18; i++)
		setasAnimation[i] =
		    createAnimation(setasTexture[i], &rect, 64, 1);

	done = 0;


	SDL_Delay(50);
	showSplashScreen();
	while (showMenu() == 1)
		game();
	
	// cleanup
	// TODO: free the structure that need to be

	// seems to create more problems with valgrind
	/*destroyAnimation(stand);
	destroyAnimation(goUp);
	destroyAnimation(goDown);
	destroyAnimation(goLeft);
	destroyAnimation(goRight);
	destroyAnimation(swordRight);*/
	for (i = 0; i < 9; i++)
	{
		destroyAnimation(setasAnimation[i]);
		SDL_DestroyTexture(setasTexture[i]);
	}
	// destroySpriteTable(groundTable);
	// free(sprites);
	// free(texturestable);

	quit(0);

	// to prevent compiler warning
	return 0;
}
