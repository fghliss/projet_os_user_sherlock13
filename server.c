#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
int culprit;  // index de la carte coupable

struct _client
{
        char ipAddress[40];
        int port;
        char name[40];
} tcpClients[4];
int nbClients;
int fsmServer;
int deck[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
int tableCartes[4][8];
char *nomcartes[]=
{"Sebastian Moran", "irene Adler", "inspector Lestrade",
  "inspector Gregson", "inspector Baynes", "inspector Bradstreet",
  "inspector Hopkins", "Sherlock Holmes", "John Watson", "Mycroft Holmes",
  "Mrs. Hudson", "Mary Morstan", "James Moriarty"};
int joueurCourant;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void melangerDeck()
{
        int i;
        int index1,index2,tmp;

        for (i=0;i<1000;i++)
        {
                index1=rand()%13;
                index2=rand()%13;

                tmp=deck[index1];
                deck[index1]=deck[index2];
                deck[index2]=tmp;
        }
}

void createTable()
{
	// Le joueur 0 possede les cartes d'indice 0,1,2
	// Le joueur 1 possede les cartes d'indice 3,4,5 
	// Le joueur 2 possede les cartes d'indice 6,7,8 
	// Le joueur 3 possede les cartes d'indice 9,10,11 
	// Le coupable est la carte d'indice 12
	int i,j,c;

	for (i=0;i<4;i++)
		for (j=0;j<8;j++)
			tableCartes[i][j]=0;

	for (i=0;i<4;i++)
	{
		for (j=0;j<3;j++)
		{
			c=deck[i*3+j];
			switch (c)
			{
				case 0: // Sebastian Moran
					tableCartes[i][7]++;
					tableCartes[i][2]++;
					break;
				case 1: // Irene Adler
					tableCartes[i][7]++;
					tableCartes[i][1]++;
					tableCartes[i][5]++;
					break;
				case 2: // Inspector Lestrade
					tableCartes[i][3]++;
					tableCartes[i][6]++;
					tableCartes[i][4]++;
					break;
				case 3: // Inspector Gregson 
					tableCartes[i][3]++;
					tableCartes[i][2]++;
					tableCartes[i][4]++;
					break;
				case 4: // Inspector Baynes 
					tableCartes[i][3]++;
					tableCartes[i][1]++;
					break;
				case 5: // Inspector Bradstreet 
					tableCartes[i][3]++;
					tableCartes[i][2]++;
					break;
				case 6: // Inspector Hopkins 
					tableCartes[i][3]++;
					tableCartes[i][0]++;
					tableCartes[i][6]++;
					break;
				case 7: // Sherlock Holmes 
					tableCartes[i][0]++;
					tableCartes[i][1]++;
					tableCartes[i][2]++;
					break;
				case 8: // John Watson 
					tableCartes[i][0]++;
					tableCartes[i][6]++;
					tableCartes[i][2]++;
					break;
				case 9: // Mycroft Holmes 
					tableCartes[i][0]++;
					tableCartes[i][1]++;
					tableCartes[i][4]++;
					break;
				case 10: // Mrs. Hudson 
					tableCartes[i][0]++;
					tableCartes[i][5]++;
					break;
				case 11: // Mary Morstan 
					tableCartes[i][4]++;
					tableCartes[i][5]++;
					break;
				case 12: // James Moriarty 
					tableCartes[i][7]++;
					tableCartes[i][1]++;
					break;
			}
		}
	}
} 

void printDeck()
{
        int i,j;

        for (i=0;i<13;i++)
                printf("%d %s\n",deck[i],nomcartes[deck[i]]);

	for (i=0;i<4;i++)
	{
		for (j=0;j<8;j++)
			printf("%2.2d ",tableCartes[i][j]);
		puts("");
	}
}

void printClients()
{
        int i;

        for (i=0;i<nbClients;i++)
                printf("%d: %s %5.5d %s\n",i,tcpClients[i].ipAddress,
                        tcpClients[i].port,
                        tcpClients[i].name);
}

int findClientByName(char *name)
{
        int i;

        for (i=0;i<nbClients;i++)
                if (strcmp(tcpClients[i].name,name)==0)
                        return i;
        return -1;
}

void sendMessageToClient(char *clientip,int clientport,char *mess)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(clientip);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(clientport);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
                printf("ERROR connecting\n");
                exit(1);
        }

        sprintf(buffer,"%s\n",mess);
        n = write(sockfd,buffer,strlen(buffer));

    close(sockfd);
}

void broadcastMessage(char *mess)
{
        int i;

        for (i=0;i<nbClients;i++)
                sendMessageToClient(tcpClients[i].ipAddress,
                        tcpClients[i].port,
                        mess);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	int i;

        char com;
        char clientIpAddress[256], clientName[256];
        int clientPort;
        int id;
        char reply[256];


     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
      srand(time(NULL));
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

	printDeck();
	melangerDeck();
	culprit = deck[12];
	printf("TEST : carte coupable désignée = « %s » (indice %d)\n",
       nomcartes[culprit], culprit);
 

	createTable();
	printDeck();
	joueurCourant=0;

	for (i=0;i<4;i++)
	{
        	strcpy(tcpClients[i].ipAddress,"localhost");
        	tcpClients[i].port=-1;
        	strcpy(tcpClients[i].name,"-");
	}

     while (1)
     {    
     	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     	if (newsockfd < 0) 
          	error("ERROR on accept");

     	bzero(buffer,256);
     	n = read(newsockfd,buffer,255);
     	if (n < 0) 
		error("ERROR reading from socket");

        printf("Received packet from %s:%d\nData: [%s]\n\n",
                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);

        if (fsmServer==0)
        {
        	switch (buffer[0])
        	{
                	case 'C':
                        	sscanf(buffer,"%c %s %d %s", &com, clientIpAddress, &clientPort, clientName);
                        	printf("COM=%c ipAddress=%s port=%d name=%s\n",com, clientIpAddress, clientPort, clientName);

                        	// fsmServer==0 alors j'attends les connexions de tous les joueurs
                                strcpy(tcpClients[nbClients].ipAddress,clientIpAddress);
                                tcpClients[nbClients].port=clientPort;
                                strcpy(tcpClients[nbClients].name,clientName);
                                nbClients++;

                                printClients();

				// rechercher l'id du joueur qui vient de se connecter

                                id=findClientByName(clientName);
                                printf("id=%d\n",id);

				// lui envoyer un message personnel pour lui communiquer son id

                                sprintf(reply,"I %d",id);
                                sendMessageToClient(tcpClients[id].ipAddress,
                                       tcpClients[id].port,
                                       reply);

				// Envoyer un message broadcast pour communiquer a tout le monde la liste des joueurs actuellement
				// connectes

                                sprintf(reply,"L %s %s %s %s", tcpClients[0].name, tcpClients[1].name, tcpClients[2].name, tcpClients[3].name);
                                broadcastMessage(reply);

				// Si le nombre de joueurs atteint 4, alors on peut lancer le jeu

                                if (nbClients == 4) {
    // 1) Envoi des 3 cartes à chaque joueur
    for (i = 0; i < 4; i++) {
        sprintf(reply, "D %d %d %d",
                deck[i*3], deck[i*3+1], deck[i*3+2]);
        sendMessageToClient(
            tcpClients[i].ipAddress,
            tcpClients[i].port,
            reply
        );
        // 2) Envoi des 8 valeurs de tableCartes
        for (int j = 0; j < 8; j++) {
            sprintf(reply, "V %d %d %d", i, j, tableCartes[i][j]);
            sendMessageToClient(
                tcpClients[i].ipAddress,
                tcpClients[i].port,
                reply
            );
        }
    }
    // 3) Annonce du joueur courant à tous
    sprintf(reply, "M %d", joueurCourant);
    broadcastMessage(reply);

    fsmServer = 1;
}
break;
}
}
else if (fsmServer == 1) {
    switch (buffer[0]) {
        case 'G': {
    int pid, guess;
    sscanf(buffer, "G %d %d", &pid, &guess);
    printf("DEBUG srv: joueur %d accuse %d\n", pid, guess);

    if (guess == culprit) {
        // 1) Envoi du message de victoire à tous
        sprintf(reply, "W %d %d", pid, culprit);
        broadcastMessage(reply);
        printf("DEBUG srv: joueur %d a gagné ! coupable = %s\n",
               pid, nomcartes[culprit]);

        // 2) Fermeture du serveur
        close(newsockfd);
        close(sockfd);
        exit(0);
    } else {
        // accusation incorrecte : ancien comportement
        broadcastMessage(buffer);
        joueurCourant = (joueurCourant + 1) % 4;
        sprintf(reply, "M %d", joueurCourant);
        broadcastMessage(reply);
        printf("DEBUG srv: passage au joueur %d\n", joueurCourant);
    }
    break;
}


        case 'O': {
    // 1) Parse
    int pid, col;
    sscanf(buffer, "O %d %d", &pid, &col);
    printf("DEBUG: exclusion objet %d par joueur %d\n", col, pid);

    // 2) Met à 0 la colonne sur tous les joueurs
    for (int k = 0; k < 4; k++) {
        tableCartes[k][col] = 0;
    }

    // 3) Envoie V… à tous
    for (int k = 0; k < 4; k++) {
        sprintf(reply, "V %d %d %d", k, col, tableCartes[k][col]);
        broadcastMessage(reply);
        printf("DEBUG: envoi %s\n", reply);
    }

    // 4) Passe au joueur suivant
    joueurCourant = (joueurCourant + 1) % 4;
    sprintf(reply, "M %d", joueurCourant);
    broadcastMessage(reply);
    printf("DEBUG: nouveau tour %d\n", joueurCourant);
    break;
}

        case 'S': {
    // Question : S <pid> <targetPlayer> <col>
    int pid, target, col;
    sscanf(buffer, "S %d %d %d", &pid, &target, &col);
    printf("DEBUG: joueur %d questionne joueur %d sur objet %d\n",
           pid, target, col);

    // Logique d’exemple : on incrémente la valeur (mod 3)
    tableCartes[target][col] = (tableCartes[target][col] + 1) % 3;

    // On envoie la mise à jour à tous
    sprintf(reply, "V %d %d %d", target, col, tableCartes[target][col]);
    broadcastMessage(reply);
    printf("DEBUG: broadcast %s\n", reply);

    // Passage au joueur suivant
    joueurCourant = (joueurCourant + 1) % 4;
    sprintf(reply, "M %d", joueurCourant);
    broadcastMessage(reply);
    printf("DEBUG: broadcast %s\n", reply);

    break;
}
sprintf(reply, "M %d", joueurCourant);
broadcastMessage(reply);
printf("DEBUG serveur: broadcast %s\n", reply);

}

}
close(newsockfd);
     }
     close(sockfd);
     return 0; 
}
