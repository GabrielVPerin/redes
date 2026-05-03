#ifndef __TYPES__
#define __TYPES__

#define MAP_SIZE 40

struct player
{
	unsigned int x;
	unsigned int y;
	unsigned int visao;
	unsigned int passos;
};

struct ghost
{
	char cor;
	unsigned int x;
	unsigned int y;
};

struct file{
	char *filename;
	unsigned int x;
	unsigned int y;
};

struct entities
{
	struct player player;
	struct ghost redGhost;
	struct ghost yellowGhost;
	struct ghost greenGhost;
	struct ghost blueGhost;
	struct file file1; 
	struct file file2; 
	struct file file3; 
	struct file file4; 
	struct file file5; 
	struct file file6; 
};

#endif