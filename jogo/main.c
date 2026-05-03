#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "terminal.h"
#include "map.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	init_terminal();

	char mapa[MAP_SIZE][MAP_SIZE];

	if (argc > 1)
		createMap(mapa, argv[argc - 1]);
	else
		createMap(mapa, NULL);
	struct player p = spawnPlayer(mapa);

	char move;
	while (1)
	{
		system("clear");
		drawMap(mapa);
		do
		{
			move = getchar();
		} while (movePlayer(&p, mapa, move));
	}

	reset_terminal();

	return 0;
}