#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common.c"
#include "sprite.c"

#define X_SETAS_INICIAL 20
#define Y_SETAS_INICIAL 50
#define X_SETAS_DISTANCIA 85

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

		SDL_Delay(TICK_INTERVAL);
		alpha = alpha + 6;
	}
}

SDL_Texture *setasTexture[9];
Animation *setasAnimation[9];

void handleKeyboard(int key, int down_or_up)
{

	/* // down
	 if(down_or_up) {
	   switch(key)
	   {
	     case SDLK_c:
	       if(draw_mode == 1) {
		 draw_mode = 0;
	       } else {
		 draw_mode = 1;
	       }
	       break;
	   }
	 }

	 // down and up
	 switch(key)
	 {
	   case SDLK_w:
	     wasd[0] = down_or_up;
	     break;
	   case SDLK_a:
	     wasd[1] = down_or_up;
	     break;
	   case SDLK_s:
	     wasd[2] = down_or_up;
	     break;
	   case SDLK_d:
	     wasd[3] = down_or_up;
	     break;
	   case SDLK_SPACE:
	     controls[0] = down_or_up;
	     break;
	 }*/
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

int main(int argc, char *argv[])
{

	int i, j, k, done;
	SDL_Event event;

	init();

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// 		Load assets
	// 		Mix_Chunk *chunk = Mix_LoadWAV("assets/swish-2.ogg");
	TTF_Font *font = getFont("assets/yoster.ttf", 26);
	// 		SDL_Texture *groundTexture =
	// getTexture("assets/ground.png");
	setasTexture[0] = getTexture("assets/seta1.png");
	setasTexture[1] = getTexture("assets/seta2.png");
	setasTexture[2] = getTexture("assets/seta3.png");
	setasTexture[3] = getTexture("assets/seta4.png");
	setasTexture[4] = getTexture("assets/seta5.png");
	setasTexture[5] = getTexture("assets/seta6.png");
	setasTexture[6] = getTexture("assets/seta7.png");
	setasTexture[7] = getTexture("assets/seta8.png");
	setasTexture[8] = getTexture("assets/seta9.png");

	// a simple channel_finished function
	void channelDone(int channelNum)
	{
		printf("Channel %d finished\n", channelNum);
		channels[channelNum] = 0;
	}
	Mix_ChannelFinished(channelDone);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 60;
	rect.h = 60;

	for (i = 0; i < 9; i++)
		setasAnimation[i] =
		    createAnimation(setasTexture[i], &rect, 30, 4);

	done = 0;

	// the current sprite respresenting the character

	int scroll_x = 0, scroll_y = 0;

	SDL_Texture *text_texture1 = NULL;

	// number of the current frame
	int frameNum = SDL_GetTicks() / TICK_INTERVAL;
	// amount of rendered frames
	int renderedFrames = 0;

	// desired frames by second
	int framesBySecond = 1000 / TICK_INTERVAL;

	// indicate if the current frame is a "real" frame
	int physical_frame = 1;

	printf("Desired fps %d\n", framesBySecond);
	printf("Start the game loop\n");

	SDL_Delay(50);
	showSplashScreen();
	// SDL_Delay(1000);
	SDL_SetRenderDrawColor(renderer, 0x70, 0xc8, 0x40, 0xff);

	int charx = viewport.w / 2;
	int chary = viewport.h / 2;

	// The game loop
	while (!done)
	{

		// slow down the physical stuff by being sure it runs
		// only once every TICK_INTERVAL, or physical frame
		if (SDL_GetTicks() / TICK_INTERVAL > frameNum)
		{
			frameNum = SDL_GetTicks() / TICK_INTERVAL;
			physical_frame = 1;
		}
		else
		{
			// no physical simulation should happen in this
			// frame
			physical_frame = 0;
		}

		// ---- Physic and events

		// this is a "real" frame where we responde to events
		// and apply physic
		if (physical_frame)
		{

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
					handleKeyboard(event.key.keysym.sym, 1);
				}

				if (event.type == SDL_KEYUP)
				{
					handleKeyboard(event.key.keysym.sym, 0);
				}
			}
		}

		// ---- Graphic rendering

		// this is not free
		SDL_RenderClear(renderer);

		/*if (!Mix_Playing(0) &&
		    Mix_PlayChannel(0, chunk, 0) == -1)
			{
				fprintf(stderr,
					"Unable to play WAV file: %s\n",
					Mix_GetError());
			}
		}*/

		drawSetasTopo(frameNum);
		// do this every second on a physical frame
		if (frameNum % framesBySecond == 0 && physical_frame)
		{

			if (text_texture1)
			{
				SDL_DestroyTexture(text_texture1);
			}

			char buffer[50];
			if (draw_mode == 1)
			{
				sprintf(buffer, "Current FPS: %d",
					renderedFrames);
			}
			else
			{
				sprintf(buffer, "Press c to cap to "
						"50FPS. Current FPS: "
						"%d",
					renderedFrames);
			}

			text_texture1 = renderFontToTexture(font, buffer);

			renderedFrames = 0;
		}

		if (text_texture1)
		{
			SDL_Rect text_rect;
			text_rect.x = 15;
			text_rect.y = 10;
			SDL_QueryTexture(text_texture1, NULL, NULL,
					 &text_rect.w, &text_rect.h);
			SDL_RenderCopy(renderer, text_texture1, NULL,
				       &text_rect);
		}

		// Update the screen
		SDL_RenderPresent(renderer);

		// Cap to ~ 50 fps
		if (draw_mode == 1)
		{
			capFramerate();
		}

		renderedFrames++;
	}

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
