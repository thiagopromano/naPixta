/*
  Copyright (C) 2013 batiste.bieler@gmail.com

  Common function and globals,
  Abstract away what is not central to the game logic
 */

#ifndef GAME_LOOP_COMMON
#define GAME_LOOP_COMMON 1

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.c"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define TICK_INTERVAL 16
#define QTD_MUSICAS 3

// this seems necessary to do this: SDL_Texture->w
// use SDL_QueryTexture(img, NULL, NULL, &w, &h); to get the size

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

struct musica
{
	int step[1000][9]; // 0 nada, 1 press, 2 long press start, 3 long press
			   // continue
	int qtdPassos;
	float tempoPorPasso;
	char nome[150];
	char musica[150];
} musicas[5];

int musicaAtual = 0;

// globals
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_DisplayMode displaymode;
SDL_Rect viewport;
int channels[4] = {0, 0, 0, 0};

GenericList *texturesList;
GenericList *musicList;
GenericList *fontList;

SDL_Color black;
SDL_Color white;

static Uint32 next_time;

inline int mod(a, b)
{
	int c = a % b;
	return (c < 0) ? c + b : c;
}

Uint32 timeLeft(void)
{
	Uint32 now;
	now = SDL_GetTicks();
	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

void capFramerate(void)
{
	SDL_Delay(timeLeft());
	next_time += TICK_INTERVAL;
}

// Call this instead of exit(), so we can clean up SDL
static void quit(int rc)
{

	printf("Cleanup\n");
	system("killall mpg123");
	ListElement *el;

	for (el = texturesList->first; el != NULL; el = el->next)
	{
		printf("Destroy texture\n");
		SDL_DestroyTexture((SDL_Texture *)el->data);
	}
	destroyList(texturesList);

	for (el = musicList->first; el != NULL; el = el->next)
	{
		printf("Destroy music\n");
		Mix_FreeMusic((Mix_Music *)el->data);
	}
	destroyList(musicList);

	for (el = fontList->first; el != NULL; el = el->next)
	{
		printf("Destroy font\n");
		TTF_CloseFont((TTF_Font *)el->data);
	}
	destroyList(fontList);

	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
		// printf("%s\n", SDL_GetError());
	}
	if (window)
	{
		SDL_DestroyWindow(window);
		// printf("%s\n", SDL_GetError());
	}
	Mix_CloseAudio();
	SDL_Quit();
	printf("Bye!\n");
	exit(0);
}

void init(void)
{

	texturesList = createList();
	musicList = createList();
	fontList = createList();
	black.r = 0;
	black.g = 0;
	black.b = 0;
	white.r = 255;
	white.g = 255;
	white.b = 255;

	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s \n",
			SDL_GetError());
		quit(1);
	}

	// Display available audio device
	int count = SDL_GetNumAudioDevices(0), i;

	fprintf(stderr, "Foram encontrados %d dispositivos de audio\n", count);
	for (i = 0; i < count; ++i)
	{
		fprintf(stderr, "Audio device %d: %s\n", i,
			SDL_GetAudioDeviceName(i, 0));
	}

	for (i = 0; i < SDL_GetNumAudioDrivers(); ++i)
	{
		const char *driver_name = SDL_GetAudioDriver(i);
		if (SDL_AudioInit(driver_name))
		{
			printf("Audio driver failed to initialize: %s\n",
			       driver_name);
			continue;
		}
		fprintf(stderr, "Consegui com o %s\n", driver_name);
	}
	/*for (i = 0; i < SDL_GetNumAudioDrivers(); ++i)
	{
		printf("Audio driver %d: %s\n", i, SDL_GetAudioDriver(i));
	}*/

	// init sound
	int audio_rate = 22050;
	Uint16 audio_format = MIX_DEFAULT_FORMAT;
	int nb_audio_channels = 4;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, nb_audio_channels,
			  audio_buffers) != 0)
	{
		fprintf(stderr, "Unable to initialize audio: %s\n",
			Mix_GetError());
		// quit(1);
	}

	// Get desktop information
	if (SDL_GetDesktopDisplayMode(0, &displaymode) < 0)
	{
		fprintf(stderr, "Could not get display mode: %s\n",
			SDL_GetError());
		quit(1);
	}

	viewport.x = 0;
	viewport.y = 0;
	viewport.w = MIN(displaymode.w, 800);
	viewport.h = MIN(displaymode.h, 600);

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
	    "Game example",		  //    window title
	    SDL_WINDOWPOS_UNDEFINED,      //    initial x destination
	    SDL_WINDOWPOS_UNDEFINED,      //    initial y destination
	    viewport.w,			  //    width, in pixels
	    viewport.h,			  //    height, in pixels
	    SDL_WINDOW_FULLSCREEN_DESKTOP //    flags
	    );

	// Check that the window was successfully made
	if (window == NULL)
	{
		// In the event that the window could not be made...
		fprintf(stderr, "Could not create window: %s\n",
			SDL_GetError());
		quit(1);
	}

	renderer =
	    SDL_CreateRenderer(window, -1, 0); // SDL_RENDERER_PRESENTVSYNC
	if (renderer < 0)
	{
		fprintf(stderr, "Could not create renderer: %s\n",
			SDL_GetError());
		quit(1);
	}

	SDL_RenderGetViewport(renderer, &viewport);

	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Unable to initialize SDL_ttf: %s \n",
			TTF_GetError());
		quit(1);
	}
}

SDL_Texture *renderFontToTexture(TTF_Font *font, char *text)
{
	// Create a text texture with a shadow effect
	SDL_Surface *textb = TTF_RenderText_Solid(font, text, black);
	SDL_Surface *textw = TTF_RenderText_Solid(font, text, white);

	/* Create a 32-bit surface with the bytes of each pixel in R,G,B,A
	   order,
	   as expected by OpenGL for textures */
	SDL_Surface *surface;
	Uint32 rmask, gmask, bmask, amask;

/* SDL interprets each pixel as a 32-bit number, so our masks must depend
   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	// with amask = 0, I see the image, with amask = 0xff000000, I see
	// nothing
	surface = SDL_CreateRGBSurface(0, textw->w + 8, textw->h + 8, 32, rmask,
				       gmask, bmask, amask);

	SDL_Rect text_rect;
	text_rect.x = 2;
	text_rect.y = 1;
	text_rect.w = textw->w;
	text_rect.h = textw->h;

	SDL_BlitSurface(textw, NULL, surface, &text_rect);
	text_rect.x = 0;
	text_rect.y = 0;
	SDL_BlitSurface(textb, NULL, surface, &text_rect);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	// This important to free the surface to avoid leaks
	SDL_FreeSurface(textw);
	SDL_FreeSurface(textb);
	SDL_FreeSurface(surface);
	return texture;
}

// Helper fonction to load assets

SDL_Texture *getTexture(char *filename)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
	if (texture == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", filename,
			SDL_GetError());
		quit(1);
	}
	addToList(texturesList, texture);
	return texture;
}

Mix_Music *getMusic(char *filename)
{
	Mix_Music *music = Mix_LoadMUS(filename);
	if (music == NULL)
	{
		fprintf(stderr, "Unable to load sound file: %s\n",
			Mix_GetError());
		quit(1);
	}
	addToList(musicList, music);
	return music;
}

TTF_Font *getFont(char *filename, int size)
{
	TTF_Font *font = TTF_OpenFont(filename, size);
	if (font == NULL)
	{
		fprintf(stderr, "Unable to load ttf file: %s\n",
			SDL_GetError());
		quit(1);
	}
	addToList(fontList, font);
	return font;
}

#endif