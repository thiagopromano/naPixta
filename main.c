#include <stdio.h>
#include <pthread.h>
#include "gpio.h"
#include "SDLFunc.h"
#define QTD_MUSICAS 5
struct passo
{
	
};

struct musica{
	int step[250][9]; //0 nada, 1 press, 2 long press start, 3 long press continue
	int qtdPassos;
	float tempoPorPasso;
	char nome [150];
}musicas[5];

void readMusic(char* arquivo, int slot)
{
	FILE * pFile;
	
	pFile = fopen (arquivo, "r");
	int linha = 0;
	fgets(musicas[slot].nome, 150, pFile);
	scanf("%f", &musicas[slot].tempoPorPasso);
	while (1){
		
		int temp;
		
		//nao escaneou mais nara = fim do arquivo
		if (scanf("%d", &temp) == 0)
		{
			break;
		}
		
		if (temp == 0)
		{
			linha++;
		}
		else
		{
			int i;
			//limpa o vetor step
			for (i = 0; i < 9; i++)
			{
				step[linha][i] = 0;
			}
			//long presss
			if (temp < 10)
				musicas[slot].step[linha][temp] = 1;
			else
			{
				int duracao = temp / 10;
				int seta = temp%10;
				musicas[slot].step[linha][seta] = 2;
				int j;
				for (j = 1; j <= duracao;  j++)
				{
					musicas[slot].step[linha+j][seta] = 3;
				}
				
			}
		}
	}
	musicas[slot].qtdPassos = linha;
	fclose(pFile);
}

void readMusics()
{
	readMusic("musics/mortovivo.pix", 0);
	
}


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