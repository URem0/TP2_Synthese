#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#define BUFSIZE 516

int main(int argc, char* argv[]){
    struct addrinfo hints;
    struct addrinfo *res;
    char host1[BUFSIZE];
    char* host = argv[1];                           // 1er argument
    char* file = argv[2];                           // 2eme argument
    int sock;
    printf("argc : %d\n",argc);
    printf("host : %s\n",host);
    printf("file : %s\n",file);

    memset(&hints, 0, sizeof(struct addrinfo));     // Initialisation de la struct hints avec que des 0
    hints.ai_family = AF_INET;

    if (argc != 3) {                                // On a ici argc = 3 car on a le nom du programme et les deux arguments.
        fprintf(stderr, "%s string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int ret = getaddrinfo(host, NULL, &hints, &res);// On récupere l'adresse du nom de domaine entrée en argument, si on veut rentrer un numero de port il suffit de d'utiliser le seconde argument de la fonction getaddrinfo

    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    
    getnameinfo(res->ai_addr, res->ai_addrlen, host1, sizeof(host1), NULL, 0, NI_NUMERICHOST);
    puts(host1);

    if ((sock=socket(AF_INET,SOCK_DGRAM,0))<0){     // On va initialiser un socket en IPv4 et avec le protocole UDP qui est le plus adapté pour notre serveur TFTP
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    freeaddrinfo(res);
    exit(EXIT_SUCCESS);
}