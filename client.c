#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#define PORT 8080



int main(){
    int sock, new_socket, valread;
    struct sockaddr_in serv_addr;
    int opt = 1, pid;
    char buffer[1024] = {0};
    char plate[] = "11오8888";
    FILE *fp;

    pid = fork();

    if(pid == 0){
	execl("./main.py","./main.py",NULL);
	return 0;
    }else{
    	sleep(5);
    }




    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET,"10.40.1.122",&serv_addr.sin_addr)<=0)
	{
		printf("address not supported \n");
		return -1;
	}

    if(connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
	    printf("\nConnection failed\n");
	    return -1;

    }

    fp = open("plate.txt",O_RDONLY);
    read(fp,buffer,10*sizeof(char));
    printf(buffer);
    send(sock, buffer, sizeof(buffer),0);
    printf(buffer);
}

