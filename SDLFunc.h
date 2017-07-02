

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
SDL_Texture *setasTexture[9];
Animation *setasAnimation[9];
int selecionado = 0;

void showSplashScreen(void)
{

	SDL_Rect rect;
	SDL_Texture *splashTexture = getTexture("assets/splash.png");
	int w, h;
	SDL_QueryTexture(splashTexture, NULL, NULL, &w, &h);
	rect.w = w;
	rect.h = h;
	rect.x = viewport.w / 2 - w / 2;
	rect.y = viewport.h / 2 - h / 2;

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

		// SDL_Delay(TICK_INTERVAL);
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
	SDL_Texture *setaMenu = getTexture("assets/setaMenu.png");
	SDL_Texture *setaMenu2 = getTexture("assets/setaMenu2.png");

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

	musicaAtual = selecionado;
	return done;
}

void handleButton(int seta, int down_or_up, long int tempoMusica)
{
	// down
	if (down_or_up == 1)
	{
		float tempoPorPasso = musicas[musicaAtual].tempoPorPasso;

		float passoAtual = (tempoMusica / 1000.0f) / tempoPorPasso;

		int iPasso = round(passoAtual);
		float diferenca = fabs(passoAtual - iPasso);
		if (diferenca < 0.3 &&
		    musicas[musicaAtual].step[iPasso][seta] != 0)
		{
			musicas[musicaAtual].step[iPasso][seta] = 0;

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

	seta = getSpriteFromAnimation(setasAnimation[0], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[3], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[6], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[7], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[4], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[1], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[2], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[5], frameNum);
	drawSpriteAt(renderer, seta, x, y);

	x += X_SETAS_DISTANCIA;
	seta = getSpriteFromAnimation(setasAnimation[8], frameNum);
	drawSpriteAt(renderer, seta, x, y);
}

drawSetasGame(int frameNum, long int tempoMusica)
{

	// int x = X_SETAS_INICIAL;
	int y = Y_SETAS_INICIAL;
	float tempoPorPasso = musicas[musicaAtual].tempoPorPasso;

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
		if (musicas[musicaAtual].step[i][0])
		{
			drawSpriteAt(renderer, setas[0], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][3])
		{
			drawSpriteAt(renderer, setas[3], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][6])
		{
			drawSpriteAt(renderer, setas[6], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][7])
		{
			drawSpriteAt(renderer, setas[7], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][4])
		{
			drawSpriteAt(renderer, setas[4], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][1])
		{
			drawSpriteAt(renderer, setas[1], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][2])
		{
			drawSpriteAt(renderer, setas[2], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][5])
		{
			drawSpriteAt(renderer, setas[5], x,
				     y + (i - passoAtual) * Y_SETAS_DISTANCIA);
		}

		x += X_SETAS_DISTANCIA;
		if (musicas[musicaAtual].step[i][8])
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

void game(void)
{
	
	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, 0x70, 0xc8, 0x40, 0xff);

	int charx = viewport.w / 2;
	int chary = viewport.h / 2;

	char buf[150];
	sprintf(buf, "mpg123 %s &", musicas[musicaAtual].musica);
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
	int score = 0;
	
	while (!done)
	{

		long int tempoMusica = SDL_GetTicks() - tempoInicial;

		// Check for events
		while (SDL_PollEvent(&event))
		{
			// printf("Event type: %d\n",
			// event.type);
			if (event.type == SDL_QUIT ||
			    event.type == SDL_WINDOWEVENT_CLOSE)
			{
				done = 1;
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
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

		drawSetasTopo(frameNum);
		drawSetasGame(frameNum, tempoMusica);

		drawScore();

		// Update the screen

		SDL_RenderPresent(renderer);

		// Cap to ~ 50 fps

		capFramerate();
		
		frameNum++;

	}
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

	setasTexture[0] = getTexture("assets/seta1.png");
	setasTexture[1] = getTexture("assets/seta2.png");
	setasTexture[2] = getTexture("assets/seta3.png");
	setasTexture[3] = getTexture("assets/seta4.png");
	setasTexture[4] = getTexture("assets/seta5.png");
	setasTexture[5] = getTexture("assets/seta6.png");
	setasTexture[6] = getTexture("assets/seta7.png");
	setasTexture[7] = getTexture("assets/seta8.png");
	setasTexture[8] = getTexture("assets/seta9.png");

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 60;
	rect.h = 60;

	for (i = 0; i < 9; i++)
		setasAnimation[i] =
		    createAnimation(setasTexture[i], &rect, 30, 4);

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
