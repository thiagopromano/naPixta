#include "gpio.h"

int botoes[] = {2,3,4,5,6,7,8,9,1};

void *rpi(void *t)
{
	int i;
	sleep(1);
	for (i = 0; i < 9; i++)
	{
		g_ativar(botoes[i], EXPORT);
		g_modo(botoes[i], INPUT);
	}
	while (1)
	{
		usleep(10000);
		for (i = 0; i < 9; i++)
		{
			if (g_read(botoes[i]))
			{
				SDL_Event ev;
				
				ev.type = SDL_KEYDOWN;
				ev.key.keysym.sym = i + SDLK_KP_1;
				SDL_PushEvent(&ev);
			}
		}
	}
}