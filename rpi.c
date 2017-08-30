#include "gpio.h"

int botoes[] = {19,26,17,22,13,27,21,5,6};
int estados [9] = {0};
void *rpi(void *t)
{
	int i;
	//sleep(1);
	for (i = 0; i < 9; i++)
	{
		g_ativar(botoes[i], EXPORT);
		g_modo(botoes[i], INPUT);
				printf("liberei %d\n",i);
	}
	while (1)
	{
		usleep(10000);
		for (i = 0; i < 9; i++)
		{
			if (g_read(botoes[i]))
			{
				if (!estados[i])
				{
					estados[i] = 1;
				SDL_Event ev;
				
				ev.type = SDL_KEYDOWN;
				ev.key.keysym.sym = i + SDLK_KP_1;
				printf("apertei %d\n",i);
				SDL_PushEvent(&ev);
					
				}
			}
			if (!g_read(botoes[i]))
			{
				if (estados[i])
				{
					estados[i] = 0;
				SDL_Event ev;
				ev.type = SDL_KEYUP;
				ev.key.keysym.sym = i + SDLK_KP_1;
				printf("soltei %d\n",i);
				SDL_PushEvent(&ev);
				}
			}
		}
	}
}