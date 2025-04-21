# projet_os_user_sherlock13

Ce document décrit pas-à-pas comment compiler, lancer et tester le serveur et les clients de ce projet Sherlock13 . Il couvre :

Prérequis

Installation et compilation

Lancement du serveur

Lancement des clients

Makefile et raccourcis

Tests fonctionnels


#1. Prérequis

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

5. Usage du Makefile

Un Makefile est disponible pour automatiser :

make ou make all : compile via ./cmd.sh.

make server : compile server.c.

make sh13 : compile sh13.c.

make clean : supprime server et sh13.

make run-server : lance ./server 5000.

make run-clients : démarre 4 clients SDL en local (Alice, Bob, Carol, Dave).


6. Tests fonctionnels clés

Étape

Résultat attendu

4 clients connectés

Serveur affiche 4 logs COM=C … name=X

Distribution cartes (D …) et grille (V …)

Chaque client voit `consomme

D …

puis 32×consomme

V …

`

Serveur broadcast M 0

Client ID=0 affiche `consomme

M 0

` et bouton Go activé

Clic Go (client 0) sur O, S ou G

Serveur log Received packet… Data: […], broadcast V et M 1 ; client 0 Go désactivé, client 1 activé

Accusation correcte (G)

Tous recoivent W <id> <culprit>, la partie se termine et SDL se ferme





