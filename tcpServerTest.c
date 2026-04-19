/* A simple server in the internet domain using TCP */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

void error(char *err_msg){
    perror(err_msg);
    exit(1);
}

void *connection_handler(void *sock_desc) {

    int sock = *(int*)sock_desc;
    int ret;
    char cli_msg[2000];

    bzero(cli_msg,2000);

    while((ret = read(sock,cli_msg,2000)) > 0){
        printf("Client message content: %s\n",cli_msg);

        ret = write(sock, "Server response: message received",34);

        if (ret < 0) error("ERROR writing to socket");
        
        bzero(cli_msg,2000);             
    }

    if (ret < 0) {
        puts("Client Disconnected: Server Closed");
        exit(1);
    }; 

    close(sock);

    return 0;
}


int main(){
    int sockfd, cli_sockfd, portno, cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = htons(2000);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");

    listen(sockfd,5);

    printf("Server Started: Listening.... \n");

    cli_len = sizeof(cli_addr);

    cli_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);

    if (cli_sockfd < 0) 
        error("ERROR on accept");
        
    puts("Client Connected");

    while(true) {
        pthread_t thread_id;
        int *thread_sockfd = malloc(1);
        *thread_sockfd = cli_sockfd;

        if (pthread_create(&thread_id, NULL, connection_handler, (void*)thread_sockfd) < 0) {
            perror("could not create thread");
            return 1;
        }

    }

    shutdown(sockfd, SHUT_RDWR);

    return 0; 
}


 