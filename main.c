// atencao, pulseaudio deve estar rodando!

#include "SDLFunc.h"
#include "gpio.h"
#include <pthread.h>
#include <stdio.h>

void readMusic(char *arquivo, int slot)
{
	FILE *pFile;
	char buf[150];
	sprintf(buf, "%s.pix", arquivo);
	pFile = fopen(buf, "r");
	if (pFile == NULL)
	{
		printf("erro ao abrir arquivo, %s", buf);
	}

	sprintf(buf, "%s.mp3", arquivo);
	strcpy(musicas[slot].musica, buf);
	int linha = 0;
	printf("lendo a linha %d", linha);
	fgets(musicas[slot].nome, 150, pFile);
	fscanf(pFile, "%f", &musicas[slot].tempoPorPasso);
	while (1)
	{

		int temp;

		// nao escaneou mais nara = fim do arquivo
		if (fscanf(pFile, "%d", &temp) == 0 || temp == -1)
		{
			break;
		}
		printf("leu %d\n", temp);
		// usleep(100000);
		if (temp == 0)
		{

			linha++;
			int i;
			// limpa o vetor step
			for (i = 0; i < 9; i++)
			{
				musicas[slot].step[linha][i] = 0;
			}
		}
		else
		{
			int i;
			// long presss
			if (temp < 10)
				musicas[slot].step[linha][temp - 1] = 1;
			else
			{
				int duracao = temp / 10;
				int seta = temp % 10;
				musicas[slot].step[linha][seta - 1] = 2;
				int j;
				for (j = 1; j <= duracao; j++)
				{
					musicas[slot]
					    .step[linha + j][seta - 1] = 3;
				}
			}
		}
	}
	printf("terminou de ler %s", musicas[slot].nome);
	musicas[slot].qtdPassos = linha;
	fclose(pFile);
}

void readMusics()
{
	readMusic("musics/enamore", 0);
	readMusic("musics/mortovivo", 1);
	readMusic("musics/gangnam", 2);
}

void main()
{

	int status = 0;
	pthread_t graficosThread;

	readMusics();

	status = pthread_create(&graficosThread, NULL, graficos, NULL);

	if (status < 0)
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