# Makefile

CC = gcc
CFLAGS = -Wall -g
SDLFLAGS = -I/usr/include/SDL2 -lSDL2_image -lSDL2_ttf -lSDL2 -lpthread

# Fichiers sources
SERVER_SRC = server.c
CLIENT_SRC = sh13.c

# Exécutables
SERVER_EXEC = server
CLIENT_EXEC = sh13

.PHONY: all clean

# Cible par défaut
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Compilation du serveur
$(SERVER_EXEC): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $(SERVER_SRC)

# Compilation du client SDL
$(CLIENT_EXEC): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $(CLIENT_SRC) $(SDLFLAGS)

# Nettoyage
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)

