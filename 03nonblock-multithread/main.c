#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <netinet/in.h>
#include <fcntl.h>

#define THREAD_SIZE 100
pthread_t thread_pool[THREAD_SIZE];


void* task(void*) {
    srand(time(NULL));
    int n = rand()%5;
    printf("[%lu] handling...\n", pthread_self());
    sleep(n);
    printf("[%lu] ok.\n", pthread_self());
    return 0;
}

int main(void) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(8000);

    if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(sock_fd, 20) < 0) {
        perror("listen");
        return -1;
    }

    fcntl(sock_fd, F_SETFL, O_NONBLOCK);

    while(1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len;
        // 等待连接
        int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd > 0) {
            // 对于每个连接，开一个线程进行处理
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, task, NULL);
            if (ret != 0) {
                printf("pthread_create failed\n");
            }
        }

        printf("do other things...\n");
        sleep(1);

    }

    return 0;
}