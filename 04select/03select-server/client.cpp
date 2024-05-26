#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

int main(int argc,char* argv[]) {
    if (argc < 2) {
        cout << "usage: <client> <port>" << endl;
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(atoi(argv[1]));

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        return -1;
    }

    while(1) {
        char buf[256]{};
        cin >> buf;
        if (send(sockfd, buf, strlen(buf),0) == -1) {
            perror("send");
            return -1;
        }
    }

    return 0;
}