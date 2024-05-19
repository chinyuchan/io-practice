#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(void) {
    for (int i=0; i < 1000; ++i) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("socket");
            return -1;
        }
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(8000);
        if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("connect");
            return -1;
        }
        char buf[256] = "hello";
        if (send(sockfd, buf, strlen(buf),0) == -1) {
            perror("send");
            return -1;
        }
    }

    return 0;
}
