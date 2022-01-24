#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char* argv[]){

    char* host = argv[1];
    char* file = argv[2];
    
    printf("argc : %d\n",argc);
    printf("host : %s\n",host);
    printf("file : %s\n",file);
}