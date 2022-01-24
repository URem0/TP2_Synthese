#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 516

int main(int argc, char* argv[]){
    struct addrinfo hints;
    struct addrinfo *res;
    char host1[BUFSIZE];
    char recvline[100];
    char* host = argv[1];                       // 1er argument adresse ip du serveur
    char* file = argv[3];                       // 3eme argument nom du fichier
    int port = atoi(argv[2]);                   // numéro de port du serveur
    
    

    char rrq[50];                               // création de la requête RRQ
    char ack[4];                                // création de l'acquittement
    int l = strlen(argv[3]);
    rrq[0] = 0;
    rrq[1] = 1;
    strcpy(rrq+2,argv[3]);
    rrq[2+strlen(argv[3])] = 0;
    strcpy(rrq+3+l,"octet");
    rrq[8+l] = 0;
    int taille_rrq = 4 + l + strlen("octet");


    int f = creat(argv[3],S_IRWXU);                              // nouveau fichier local 

    printf("argc : %d\n",argc);
    printf("host : %s\n",host);
    printf("port: %d \n",port);
    printf("file : %s\n",file);

    memset(&hints, 0, sizeof(struct addrinfo));     // Initialisation de la struct hints avec que des 0
    hints.ai_family = AF_INET;                      // protocol ipv4
    hints.ai_socktype =SOCK_DGRAM;                  // envoie de datagramme car en udp
    hints.ai_protocol = 0;

    if (argc != 4) {                                // On a ici argc = 4 car on a le nom du programme et les trois arguments.
        fprintf(stderr, "%s string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int ret = getaddrinfo(argv[1], argv[2], &hints, &res); 

    if (ret != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    
    getnameinfo(res->ai_addr, res->ai_addrlen, host1, sizeof(host1), NULL, 0, NI_NUMERICHOST);
   
    
    int sock = socket(AF_INET,SOCK_DGRAM,0);

    if (sock<0){     
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    sendto(sock,rrq,taille_rrq,0,res->ai_addr,res->ai_addrlen);
     while(1){
		int len = recvfrom(sock,recvline,BUFSIZE,0,(res->ai_addr),&res->ai_addrlen);
		char s[10];
		sprintf(s,"%d",len);
		write(1,s,5);
		ack[0] = 0;       //acquittement
		ack[1] = 4;
		ack[2] = recvline[2];
		ack[3] = recvline[3];
        sendto(sock,ack,4,0,(res->ai_addr),res->ai_addrlen);
        if(len > 4){
			write(f,recvline+4,len-4);          //écriture dans le fichier
			write(1,"writing\n",8);
		}
        if(len < BUFSIZE){
			break;
		}
	}
	close(f);
    
	printf("receive: %s\n",recvline);	
    
    freeaddrinfo(res);
    exit(EXIT_SUCCESS);
}