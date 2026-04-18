/* A simple server in the internet domain using TCP */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(){
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

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

    printf("Server started: Listening.... \n");

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) 
        error("ERROR on accept");

    bzero(buffer,256);

    while(true){
        n = read(newsockfd,buffer,255);

        if (n < 0) {
            error("ERROR reading from socket");
            break;
        }

        printf("Client message content: %s\n",buffer);

        n = write(newsockfd, "Server response: message received",34);

        if (n < 0) error("ERROR writing to socket");
    }

    close(newsockfd);

    shutdown(sockfd, SHUT_RDWR);

    return 0; 
}

 