# projet_os_user_sherlock13

Ce document décrit pas-à-pas comment compiler, lancer et tester le serveur et les clients de ce projet Sherlock13 . Il couvre :

Prérequis

Installation et compilation

Lancement du serveur

Lancement des clients

Tests fonctionnels

1. Prérequis

Système : Linux 

Compilateur : gcc (>= 9.0)

SDL2 et modules :

sudo apt-get update
sudo apt-get install build-essential \
     libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

Bibliothèques POSIX : pthreads, sockets TCP.

Fichiers à avoir dans le même répertoire :

server.c

sh13.c (client SDL)

cmd.sh (script de compilation)

Images du jeu (SH13_0.png … SH13_12.png, objets, boutons)

Police sans.ttf

2. Installation & Compilation

Rends le script exécutable :

chmod +x cmd.sh

Lance la compilation :

./cmd.sh

Génère deux exécutables : server et sh13.

Si vous préférez compiler manuellement :

gcc -o server server.c
gcc -o sh13 -I/usr/include/SDL2 sh13.c \
    -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread

3. Lancer le serveur

Dans un terminal, exécutez :

./server <port_serveur>
# Exemple :
./server 5000

Le serveur affiche alors le deck mélangé et attend les connexions des clients.
