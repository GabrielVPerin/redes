#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "entities.h"
#include "terminal.h"
#include "map.h"
#include "types.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	init_terminal();

	char mapa[MAP_SIZE][MAP_SIZE];

	if (argc > 1)
		createMap(mapa, argv[argc - 1]);
	else
		createMap(mapa, NULL);

	struct entities entities = spawnEntities(mapa);
	struct pacman pacMan = entities.pacman;

	char move;
	FILE *enviarCliente = fopen("pacotaoDoPerin.csv", "w+");
	if (!enviarCliente)
		return 1;

	while (1)
	{
		system("clear");
		// drawMap(mapa, pacMan);
		// printf("\n\n");
		char **mapView = drawPacmanView(mapa, pacMan);

		// FILE *enviarCliente = fopen("pacotaoDoPerin.csv", "w+");

		// unsigned int nPacote = (pacMan.visao * 2 + 1) << 1;
		fseek(enviarCliente, 0, SEEK_SET);
		for (int i = 0; i < (int)pacMan.visao * 2 + 1; i++)
		{
			fwrite(mapView[i], sizeof(char), pacMan.visao * 2 + 1, enviarCliente);
			fputc('\n', enviarCliente);
		}
		for (int i = 0; i < ((int)pacMan.visao * 2 + 1); i++)
		{
			int flag = 0;
			for (int j = 0; j < ((int)pacMan.visao * 2 + 1); j++)
			{
				switch (mapView[i][j])
				{
				case 'X':
					printf("# ");
					flag = 1;
					break;
				case 'V':
					break;
				case 'E':
				case '0':
					printf("  ");
					flag = 1;
					break;
				default:
					printf("%c ", mapView[i][j]);
					flag = 1;
					break;
				}
			}
			if (flag)
				printf("\n");
			free(mapView[i]);
		}
		free(mapView);
		do
		{
			move = getchar();
		} while (movePacman(&pacMan, mapa, move));
		moveAllGhosts(mapa, &entities);
	}

	fclose(enviarCliente);
	reset_terminal();

	return 0;
}