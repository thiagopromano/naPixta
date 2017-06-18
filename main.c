#include <stdio.h>
#include <pthread.h>
#include "gpio.h"
#include "SDLFunc.h"


void main()
{
    
    int status = 0;
	pthread_t graficosThread;
    
    status = pthread_create( &graficosThread,
				NULL,
				graficos,
				NULL);
				
    if( status < 0 )
	{
		perror("Deu ruim na thread de gráficos!");
		return;
	}
	
	
	while (1)
	{
	    usleep(100000);
	}
    return;
}