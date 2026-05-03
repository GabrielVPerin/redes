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
	struct player pac = spawnPlayer(mapa);

	char move;
	while (1)
	{
		system("clear");
		drawMap(mapa, pac);
		do
		{
			move = getchar();
		} while (movePlayer(&pac, mapa, move));
	}

	reset_terminal();

	return 0;
}