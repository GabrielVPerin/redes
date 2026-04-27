CC = gcc
CFLAGS = -Wall -g -I./utilidades

UTILS_DIR = utilidades
CLIENTE_DIR = cliente
SERVER_DIR = servidor

UTILS_SRC = $(wildcard $(UTILS_DIR)/*.c)
CLIENTE_SRC = $(wildcard $(CLIENTE_DIR)/*.c)
SERVER_SRC = $(wildcard $(SERVER_DIR)/*.c)

UTILS_OBJ = $(UTILS_SRC:.c=.o)
CLIENTE_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)

CLIENTE_BIN = envia
SERVER_BIN = recebe

all: $(CLIENTE_BIN) $(SERVER_BIN)

$(CLIENTE_BIN): $(CLIENTE_OBJ) $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_BIN): $(SERVER_OBJ) $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(UTILS_OBJ) $(CLIENTE_OBJ) $(SERVER_OBJ) recebe envia
