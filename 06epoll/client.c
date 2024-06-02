#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: <Client> <IP> <Port>\n");
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        return -1;
    }
    char buf[256] = {0};
    while(1) {
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        if (send(sockfd, buf, strlen(buf)-1, 0) == -1) {
            perror("send");
            return -1;
        }
        if (!strcmp(buf, "quit\n")) {
            break;
        }
    }

    close(sockfd);
    return 0;
}

