# Sherlock13 – Guide d’installation et de tests

Ce projet implémente un jeu de déduction collaboratif inspiré de “Clue” : 4 joueurs, 13 suspects, 8 objets, 1 coupable à découvrir.

---

## Sommaire

1. [Prérequis](#1-prérequis)  
2. [Installation & compilation](#2-installation--compilation)  
3. [Lancement du serveur](#3-lancement-du-serveur)  
4. [Lancement des clients](#4-lancement-des-clients)  
5. [Makefile & raccourcis](#5-makefile--raccourcis)  
6. [Tests fonctionnels clés](#6-tests-fonctionnels-clés)  
7. [Debug & logs](#7-debug--logs)  
8. [Dépannage SDL](#8-dépannage-sdl)  

---

## 1. Prérequis

- **OS** : Linux (Ubuntu/Debian)  
- **GCC** : ≥ 9.0  
- **SDL2** + extensions :
  ```bash
  sudo apt-get update
  sudo apt-get install build-essential \
       libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
