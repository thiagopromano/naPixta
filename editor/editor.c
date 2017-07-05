#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include <sys/time.h>


float segundoAtual = 0;
long long timestampInicial;
int passoImpresso = 0;

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

int pinos [9] = {1,2,3,4,5,6,7,8,9};
int estados [9] = {0};
int main()
{
    float ritmo;
    float duracao;
    char vetor[50];
	printf("Diga o nome da musica: ");
	/*char c;
	while ((c = getchar()) != '\n' && c != EOF)
	{
	}*/
	gets(vetor);
	
    printf ("diga o ritmo em segundos\n");
    scanf("%f", &ritmo);
    printf ("diga a duracao em segundos\n");
    scanf("%f", &duracao);
    int i;
    for (i = 0; i < 9; i++)
    {
        g_ativar(pinos[i], EXPORT);
        g_modo(pinos[i], INPUT);
    }
    FILE *fp = fopen("musica.pix", "w");
    
    fprintf(fp, "%s\n", vetor);
    fprintf(fp, "%f\n", duracao);
    fprintf(fp, "%f\n", ritmo);
    system("mpg123 enamore.mp3 &");
    timestampInicial = current_timestamp();
    
    while(segundoAtual < duracao)
    {
        segundoAtual = (current_timestamp() - timestampInicial) / 1000.0f;
        
        float passoAtual = segundoAtual/ritmo;
        
        if (passoImpresso < passoAtual- ritmo/2 && passoAtual > 1.6f)
        {
            passoImpresso++;
            fprintf(fp, "0\n");
        }
        usleep(1000); // nao consumir toda a cpu
        /*for(i = 0; i < 9; i++)
        {
            if (g_read(i))
            {
                if (!estados[i])
                {
                    estados[i]= 1;
                    fprintf(fp, "%d ", i);
                }
            }
            else
            {
                estados[i] = 0;
            }
        }*/
        while(kbhit())
        {
            char entrada = getchar();
            if (entrada > '0' && entrada <= '9')
            {
                fprintf(fp, "%c ", entrada);
            }
        }
    }
    fprintf(fp, "\n-1");
    system("killall mpg123");
    fclose(fp);
}