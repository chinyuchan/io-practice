#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

// 监听标准输入，若有输入则打印，否则等待超时，返回0
int main() {
    fd_set read_fds;

    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);
    struct timeval timeout;
    while(1) {
        // 超时时间
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        fd_set temp_fds = read_fds;
        int nready = select(1, &temp_fds, NULL, NULL, &timeout);
        printf("ready: %d\n", nready);

        if (nready == -1) {
            perror("select");
            break;
        }
        else if (!nready) {
            puts("timeout");
        }
        else {
            if (FD_ISSET(0, &temp_fds)) {
                char buf[BUF_SIZE+1] = {0};
                int n = read(0, buf, BUF_SIZE);
                buf[n] = 0;
                printf("receive from %d bytes from std input: %s", n, buf);
            }
        }
    }


    return 0;
}