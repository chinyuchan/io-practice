#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    char buff[1024] = { 0 };
    int fd = open("/home/chinyuchan/Downloads/ideaIU-2024.1.1.tar.gz", O_RDONLY, 06666);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    ssize_t sz;
    while((sz = read(fd, buff, sizeof(buff))) > 0) {
        printf("<<<<<: %s\n", buff);
    }
    if (sz < 0) {
        perror("read");
        return -1;
    }


    return 0;
}
