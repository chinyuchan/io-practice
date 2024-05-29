#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <poll.h>

int main(void) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    printf("sockfd=%d\n", fd);

    int enable_reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int)) < 0) {
        perror("setsockopt");
       return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(8000);

    if (bind(fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return -1;
    }

    if (listen(fd, 20) < 0) {
        perror("listen");
        return -1;
    }

    nfds_t num = 32;
    struct pollfd* fds = malloc(num * sizeof(struct pollfd));
    for (nfds_t i = 0; i < num; ++i) {
        fds[i].fd = i;
        fds[i].events = POLLIN;
    }
    char buf[256];
    ssize_t sz;
    while(1) {
        int ready = poll(fds, num, -1);
        if (ready < 0) {
            perror("ready");
            return -1;
        }

        for (nfds_t j = 0; j < num; ++j) {
            if (fds[j].revents != 0) {
                if (fds[j].revents & POLLIN) {
                    if (fds[j].fd == fd) {
                        struct sockaddr addr;
                        socklen_t len;
                        int new_fd = accept(fds[j].fd, &addr, &len);
                        printf("new fd: %d\n", new_fd);
                    }
                    else {
                        sz = recv(fds[j].fd, buf, sizeof(buf), MSG_DONTWAIT);
                        if (sz < 0) {
                            perror("recv");
                            return -1;
                        }
                        if (!sz) {
                            printf("peer closed: %d\n", fds[j].fd);
                            fds[j].fd = -1;
                            continue;
                        }
                        buf[sz] = '\0';
                        printf("[%ld]<<<<<<<<<<<: %s\n", sz, buf);
                    }
                }

            }
            // else {
            //     printf("do other things...\n");
            // }
        }
    }

    return 0;
}
