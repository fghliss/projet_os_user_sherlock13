# Sherlock13 – Guide d’installation et de tests

Ce projet implémente un jeu de déduction collaboratif inspiré de **Clue** :  
4 joueurs, 13 suspects, 8 objets, 1 coupable à découvrir.

---

## Sommaire

1. [Prérequis](#1-prérequis)  
2. [Installation & compilation](#2-installation--compilation)  
3. [Lancement du serveur](#3-lancement-du-serveur)  
4. [Lancement des clients](#4-lancement-des-clients)  
5. [Makefile & raccourcis](#5-makefile--raccourcis)  
6. [Tests fonctionnels clés](#6-tests-fonctionnels-clés)  


---

## 1. Prérequis

- **OS** : Linux (Ubuntu/Debian)  
- **GCC** : version 9.0 ou supérieure  
- **SDL2** + extensions :  
  ```bash
  sudo apt-get update
  sudo apt-get install build-essential \
       libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
  ```  
- **POSIX** : pthreads, sockets TCP  
- **Ressources** (dans un même dossier) :  
  - `server.c`  
  - `sh13.c` (client SDL)  
  - `cmd.sh` (script de compilation)  
  - Images (`SH13_0.png`…`SH13_12.png`, icônes objets, boutons)  
  - Police `sans.ttf`

## 2. Installation & compilation

1. Rendre le script exécutable :  
   ```bash
   chmod +x cmd.sh
   ```  
2. Lancer la compilation :  
   ```bash
   ./cmd.sh
   ```  
   → génère `server` et `sh13`

> **Compilation manuelle** :  
> ```bash
> gcc -o server server.c
> gcc -o sh13 -I/usr/include/SDL2 sh13.c \
>     -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread
> ```

## 3. Lancement du serveur

```bash
./server <port>
# ex. : ./server 5000
```

Le serveur affiche le deck mélangé et attend **4** connexions.

## 4. Lancement des clients

Ouvrez **4** terminaux et, dans chacun :  
```bash
./sh13 <ip_serveur> <port_serveur> \
      <ip_client>  <port_client>  <NomJoueur>
```
- `<ip_serveur>:<port_serveur>` = `localhost 5000`  
- `<ip_client>:<port_client>` = ports distincts `5001`→`5004`  
- `<NomJoueur>` = pseudonyme (Alice, Bob, …)

```bash
./sh13 localhost 5000 localhost 5001 Alice
./sh13 localhost 5000 localhost 5002 Bob
./sh13 localhost 5000 localhost 5003 Carol
./sh13 localhost 5000 localhost 5004 Dave
```

Cliquez sur **Connect** dans chaque fenêtre SDL.

## 5. Makefile & raccourcis

Le dépôt inclut un **Makefile** :  

- make ou make all → compile les deux exécutables server et sh13

- make clean        → supprime server et sh13

## 6. Tests fonctionnels clés

**6.1 4 clients connectés**  
Le serveur doit afficher **4 fois** :  
```
COM=C ipAddress=… port=… name=<NomJoueur>
```

**6.2 Distribution des cartes (`D …`)**  
Chaque client doit afficher :  
```
consomme |D <c0> <c1> <c2>|
```

**6.3 Distribution de la grille (`V …`)**  
Chaque client doit afficher **32 fois** :  
```
consomme |V <joueur> <col> <valeur>|
```

**6.4 Premier tour (`M 0`)**  
Le client **ID 0** doit afficher :  
```
consomme |M 0|
```  
et voir le bouton **Go** activé.

**6.5 Clic Go (client 0) sur `O`, `S` ou `G`**  
1. **Serveur** logue :  
   ```
   Received packet… Data: [O 0 …]  // ou [S 0 …] / [G 0 …]
   ```  
2. **Serveur** broadcast : `V …` puis `M 1`  
3. **Client 0** désactive Go ; **Client 1** active Go

**6.6 Accusation correcte (`G`)**  
Tous les clients reçoivent :  
```
W <id> <culprit>
```  
La partie se termine et SDL se ferme.


