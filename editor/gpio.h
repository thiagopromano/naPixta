#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define EXPORT 1
#define UNEXPORT 0
#define INPUT 1
#define OUTPUT 0
#define HIGH 1
#define LOW 0

int g_ativar(int pino, int unouex)
{
	char buffer[3];
	int arquivo;

	arquivo = open((unouex == 0) ? "/sys/class/gpio/unexport"
				     : "/sys/class/gpio/export",
		       O_WRONLY);
	if (arquivo == -1)
		return -1;

	snprintf(buffer, 3, "%d", pino);
	if (write(arquivo, buffer, 3) == -1)
	{
		close(arquivo);
		return -2;
	}

	close(arquivo);
	return 1;
}

int g_modo(int pino, int direcao)
{
	char caminho[35];
	int arquivo;

	snprintf(caminho, 35, "/sys/class/gpio/gpio%d/direction", pino);
	arquivo = open(caminho, O_WRONLY);
	if (arquivo == -1)
		return -1;

	if (write(arquivo, (direcao == 0) ? "out" : "in", 3) == -1)
	{
		close(arquivo);
		return -2;
	}

	close(arquivo);
	return 1;
}

int g_write(int pino, int estado)
{
	int arquivo;
	char caminho[33];

	snprintf(caminho, 33, "/sys/class/gpio/gpio%d/value", pino);
	arquivo = open(caminho, O_WRONLY);
	if (arquivo == -1)
		return -1;

	if (write(arquivo, (estado == 0) ? "0" : "1", 1) == -1)
	{
		close(arquivo);
		return -2;
	}
	close(arquivo);
	return 1;
}

int g_read(int pino)
{
	int arquivo;
	char caminho[33];
	char retorno[3];

	snprintf(caminho, 33, "/sys/class/gpio/gpio%d/value", pino);
	arquivo = open(caminho, O_RDONLY);
	if (arquivo == -1)
		return -1;

	if (read(arquivo, retorno, 3) == -1)
	{
		close(arquivo);
		return -2;
	}

	close(arquivo);
	return ((int)retorno[0] - 48);
}

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}