#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_NUM 32

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: <Server> <Port>\n");
        return -1;
    }

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[1]));
    serveraddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(listenfd, 20) == -1) {
        perror("listen");
        return -1;
    }
    // 创建epoll实例
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return -1;
    }
    // 将listenfd加入到epfd进行监控
    struct epoll_event ev;
    ev.data.fd = listenfd;
    ev.events = EPOLL_CTL_ADD;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        perror("epoll_ctl");
        return -1;
    }
    int i;
    struct epoll_event events[MAX_NUM]; // 用于存放就绪的fd的event
    char buf[256] = {0};
    while(1) {
        // 等待就绪的fd
        int ready = epoll_wait(epfd, events, MAX_NUM, -1);
        if (ready == -1) {
            perror("epoll_wait");
            return -1;
        }

        for (i = 0; i < ready; ++i) {
            if (events[i].data.fd == listenfd) { // 有新连接进来
                struct sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                int conn_fd = accept(listenfd,(struct sockaddr*)&addr, &addrlen);
                if (conn_fd == -1) {
                    perror("accept");
                    return -1;
                }
                printf("new connection: fd=%d\n", conn_fd);
                // 将新的fd加入监控
                bzero(&ev, sizeof(ev));
                ev.data.fd = conn_fd;
                ev.events = EPOLLIN;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
                    perror("epoll_ctl");
                    return -1;
                }
            }
            else { // 其他的fd就绪
                bzero(buf, sizeof(buf));
                int fd = events[i].data.fd;
                int nbytes = recv(fd, buf, sizeof(buf), 0);
                if (nbytes == -1) {
                    perror("recv");
                    return -1;
                }
                if (nbytes == 0) {
                    // 从epfd中删除
                    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                        perror("epoll_ctl");
                        return -1;
                    }
                    close(fd);
                    printf("client disconnted: fd=%d\n", fd);
                    continue;
                }
                buf[nbytes] = '\0';
                if (!strcmp(buf, "quit")) {
                    printf("client disconnted: fd=%d\n", fd);
                    close(fd);
                    continue;
                }
                printf("recv %03d bytes from fd(%d) : %s\n", nbytes, fd, buf);
            }
        }
    }
    return 0;
}


