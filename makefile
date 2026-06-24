CC = gcc
CFLAGS = -Wall -g -I./utilidades -I./pacman

UTILS_DIR = utilidades
CLIENTE_DIR = cliente
SERVER_DIR = servidor
JOGO_DIR = pacman

UTILS_SRC = $(wildcard $(UTILS_DIR)/*.c)
CLIENTE_SRC = $(wildcard $(CLIENTE_DIR)/*.c)
SERVER_SRC = $(wildcard $(SERVER_DIR)/*.c)
JOGO_SRC = $(wildcard $(JOGO_DIR)/*.c)

UTILS_OBJ = $(UTILS_SRC:.c=.o)
CLIENTE_OBJ = $(CLIENTE_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
JOGO_OBJ = $(JOGO_SRC:.c=.o)

CLIENTE_BIN = client
SERVER_BIN = server

all: $(CLIENTE_BIN) $(SERVER_BIN)

$(CLIENTE_BIN): $(CLIENTE_OBJ) $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_BIN): $(SERVER_OBJ) $(UTILS_OBJ) $(JOGO_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(UTILS_OBJ) $(CLIENTE_OBJ) $(SERVER_OBJ) $(JOGO_OBJ)
	rm -f $(CLIENTE_BIN) $(SERVER_BIN)
	rm -f pacotaoDoPerin.csv
