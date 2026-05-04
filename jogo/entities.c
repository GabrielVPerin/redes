#include <stdlib.h>
#include "entities.h"
#include "terminal.h"

void randSpawn(unsigned int *x, unsigned int *y, char mapa[MAP_SIZE][MAP_SIZE])
{
    do
    {
        *x = rand() % MAP_SIZE;
        *y = rand() % MAP_SIZE;
    } while (mapa[*y][*x] != '0');
}

struct entities spawnEntities(char mapa[MAP_SIZE][MAP_SIZE])
{
    struct entities entidades = {0};
    int flags[11] = {0};

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            switch (mapa[i][j])
            {
            case 'P':
                entidades.pacman.x = j;
                entidades.pacman.y = i;
                flags[0] = 1;
                break;
            case 'B':
                entidades.blueGhost.x = j;
                entidades.blueGhost.y = i;
                flags[1] = 1;
                break;
            case 'R':
                entidades.redGhost.x = j;
                entidades.redGhost.y = i;
                flags[2] = 1;
                break;
            case 'Y':
                entidades.yellowGhost.x = j;
                entidades.yellowGhost.y = i;
                flags[3] = 1;
                break;
            case 'G':
                entidades.greenGhost.x = j;
                entidades.greenGhost.y = i;
                flags[4] = 1;
                break;
            case '1':
                entidades.file1.x = j;
                entidades.file1.y = i;
                flags[5] = 1;
                break;
            case '2':
                entidades.file2.x = j;
                entidades.file2.y = i;
                flags[6] = 1;
                break;
            case '3':
                entidades.file3.x = j;
                entidades.file3.y = i;
                flags[7] = 1;
                break;
            case '4':
                entidades.file4.x = j;
                entidades.file4.y = i;
                flags[8] = 1;
                break;
            case '5':
                entidades.file5.x = j;
                entidades.file5.y = i;
                flags[9] = 1;
                break;
            case '6':
                entidades.file6.x = j;
                entidades.file6.y = i;
                flags[10] = 1;
                break;
            }
        }
    }

    for (int i = 0; i < 11; i++)
    {
        if (!flags[i])
        {
            unsigned int x, y;
            randSpawn(&x, &y, mapa);

            switch (i)
            {
            case 0:
                entidades.pacman.x = x;
                entidades.pacman.y = y;
                mapa[y][x] = 'P';
                break;
            case 1:
                entidades.blueGhost.x = x;
                entidades.blueGhost.y = y;
                mapa[y][x] = 'B';
                break;
            case 2:
                entidades.redGhost.x = x;
                entidades.redGhost.y = y;
                mapa[y][x] = 'R';
                break;
            case 3:
                entidades.yellowGhost.x = x;
                entidades.yellowGhost.y = y;
                mapa[y][x] = 'Y';
                break;
            case 4:
                entidades.greenGhost.x = x;
                entidades.greenGhost.y = y;
                mapa[y][x] = 'G';
                break;
            case 5:
                entidades.file1.x = x;
                entidades.file1.y = y;
                mapa[y][x] = '1';
                break;
            case 6:
                entidades.file2.x = x;
                entidades.file2.y = y;
                mapa[y][x] = '2';
                break;
            case 7:
                entidades.file3.x = x;
                entidades.file3.y = y;
                mapa[y][x] = '3';
                break;
            case 8:
                entidades.file4.x = x;
                entidades.file4.y = y;
                mapa[y][x] = '4';
                break;
            case 9:
                entidades.file5.x = x;
                entidades.file5.y = y;
                mapa[y][x] = '5';
                break;
            case 10:
                entidades.file6.x = x;
                entidades.file6.y = y;
                mapa[y][x] = '6';
                break;
            }
        }
    }

    entidades.pacman.passos = 0;
    entidades.pacman.visao = 100;

    entidades.blueGhost.cor = 'B';
    entidades.blueGhost.isFacing = 1;

    entidades.redGhost.cor = 'R';
    entidades.redGhost.isFacing = 1;

    entidades.yellowGhost.cor = 'Y';
    entidades.yellowGhost.isFacing = 1;

    entidades.greenGhost.cor = 'G';
    entidades.greenGhost.isFacing = 1;
    entidades.greenGhost.lastTurn = 1;

    entidades.file1.filename = "1.txt";
    entidades.file2.filename = "2.txt";
    entidades.file3.filename = "3.jpg";
    entidades.file4.filename = "4.jpg";
    entidades.file5.filename = "5.mp4";
    entidades.file6.filename = "6.mp4";

    return entidades;
}

int movePacman(struct pacman *p, char mapa[MAP_SIZE][MAP_SIZE], char direcao)
{
    if (mapa[p->y][p->x] != 'P')
    {
        reset_terminal();
        exit(1);
    }

    struct pacman aux = *p;

    switch (direcao)
    {
    case 'w':
        aux.y--;
        break;
    case 'a':
        aux.x--;
        break;
    case 's':
        aux.y++;
        break;
    case 'd':
        aux.x++;
        break;
    default:
        break;
    }

    if (mapa[aux.y][aux.x] == 'X')
        return 1;
    mapa[p->y][p->x] = '0';
    *p = aux;
    mapa[p->y][p->x] = 'P';
    p->passos++;
    if (p->passos % 5 == 0)
        p->visao++;

    return 0;
}

int pacmanView(int x, int y, struct pacman p)
{
    int dx = x - (int)p.x;
    int dy = y - (int)p.y;
    int r = (int)p.visao;

    return (dx * dx + dy * dy) <= (r * r + r);
}


// 1 up / -1 down / 2 left / -2 right

int dx(int dir)
{
    if (dir == 2)
        return -1;
    if (dir == -2)
        return 1;
    return 0;
}

int dy(int dir)
{
    if (dir == 1)
        return -1;
    if (dir == -1)
        return 1;
    return 0;
}

int turnLeft(int dir)
{
    if (dir == 1)
        return 2; // up -> left
    if (dir == 2)
        return -1; // left -> down
    if (dir == -1)
        return -2; // down -> right
    if (dir == -2)
        return 1; // right -> up

    exit(1);
}

int turnRight(int dir)
{
    if (dir == 1)
        return -2;
    if (dir == -2)
        return -1;
    if (dir == -1)
        return 2;
    if (dir == 2)
        return 1;

    exit(1);
}

int turnBack(int dir)
{
    return -dir;
}

int canMove(char mapa[MAP_SIZE][MAP_SIZE], int x, int y)
{
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
        return 0;

    return mapa[y][x] != 'X' && !(mapa[y][x] >= '1' && mapa[y][x] <= '6');
}

void moveRedGhost(char mapa[MAP_SIZE][MAP_SIZE], struct ghost *gR)
{
    struct ghost old = *gR;

    int dir = gR->isFacing;

    int dirs[4] = {
        dir,
        turnLeft(dir),
        turnRight(dir),
        turnBack(dir)};

    for (int i = 0; i < 4; i++)
    {
        int newDir = dirs[i];
        int nx = gR->x + dx(newDir);
        int ny = gR->y + dy(newDir);

        if (canMove(mapa, nx, ny))
        {
            gR->x = nx;
            gR->y = ny;
            gR->isFacing = newDir;
            break;
        }
    }

    mapa[old.y][old.x] = '0';
    mapa[gR->y][gR->x] = 'R';
}

void moveBlueGhost(char mapa[MAP_SIZE][MAP_SIZE], struct ghost *gB)
{
    struct ghost old = *gB;

    int dir = gB->isFacing;

    int dirs[4] = {
        turnRight(dir),
        dir,
        turnLeft(dir),
        turnBack(dir)};

    for (int i = 0; i < 4; i++)
    {
        int newDir = dirs[i];
        int nx = gB->x + dx(newDir);
        int ny = gB->y + dy(newDir);

        if (canMove(mapa, nx, ny))
        {
            gB->x = nx;
            gB->y = ny;
            gB->isFacing = newDir;
            break;
        }
    }

    mapa[old.y][old.x] = '0';
    mapa[gB->y][gB->x] = 'B';
}

void moveGreenGhost(char mapa[MAP_SIZE][MAP_SIZE], struct ghost *gG)
{
    struct ghost old = *gG;

    int dir = gG->isFacing;

    int dirs[4];

    if (gG->lastTurn)
    {
        dirs[0] = turnRight(dir);
        dirs[1] = dir;
        dirs[2] = turnLeft(dir);
        dirs[3] = turnBack(dir);
    }
    else
    {
        dirs[0] = turnLeft(dir);
        dirs[1] = dir;
        dirs[2] = turnRight(dir);
        dirs[3] = turnBack(dir);
    }

    for (int i = 0; i < 4; i++)
    {
        int newDir = dirs[i];
        int nx = gG->x + dx(newDir);
        int ny = gG->y + dy(newDir);

        if (canMove(mapa, nx, ny))
        {
            gG->x = nx;
            gG->y = ny;
            gG->isFacing = newDir;

            if (newDir != dir)
                gG->lastTurn = !gG->lastTurn;

            break;
        }
    }

    mapa[old.y][old.x] = '0';
    mapa[gG->y][gG->x] = 'G';
}

void moveYellowGhost(char mapa[MAP_SIZE][MAP_SIZE], struct ghost *gY)
{
    struct ghost old = *gY;

    int dir = gY->isFacing;

    int dirs[4] = {
        turnLeft(dir),
        dir,
        turnRight(dir),
        turnBack(dir)};

    for (int i = 0; i < 3; i++)
    {
        int j = rand() % (i + 1);

        int temp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = temp;
    }

    for (int i = 0; i < 4; i++)
    {
        int newDir = dirs[i];
        int nx = gY->x + dx(newDir);
        int ny = gY->y + dy(newDir);

        if (canMove(mapa, nx, ny))
        {
            gY->x = nx;
            gY->y = ny;
            gY->isFacing = newDir;
            break;
        }
    }

    mapa[old.y][old.x] = '0';
    mapa[gY->y][gY->x] = 'Y';
}

void moveAllGhosts(char mapa[MAP_SIZE][MAP_SIZE], struct entities *entities)
{
    moveRedGhost(mapa, &entities->redGhost);
    moveBlueGhost(mapa, &entities->blueGhost);
    moveGreenGhost(mapa, &entities->greenGhost);
    moveYellowGhost(mapa, &entities->yellowGhost);

}
