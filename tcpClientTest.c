#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void error(char *err_msg){
    perror(err_msg);
    exit(0);
}

int main(){
    int sockfd, portno, ret;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char msg_buf[2000];

    portno = htons(2000);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname("127.0.0.1");

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr_list[0], 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Enter message (type 'exit' to close): ");

    bzero(msg_buf,2000);

    while (true) {
        fgets(msg_buf,2000,stdin);

            // Remove new line character if present
        msg_buf[strcspn(msg_buf, "\n")] = '\0';

        if (strcmp(msg_buf, "exit") == 0) {
            break;
        }

        ret = write(sockfd,msg_buf,strlen(msg_buf));

        if (ret < 0) 
            error("ERROR writing to socket");

        bzero(msg_buf,2000);

        ret = read(sockfd,msg_buf,2000);

        if (ret < 0) 
            error("ERROR reading from socket");

        printf("%s\n",msg_buf);
    }
    
    close(sockfd);

    return 0;
}


