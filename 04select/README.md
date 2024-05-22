# Select模型
select需要使用两个system call(`select`和`recvfrom`)，而blocking I/O只调用了一个system call(`recvfrom`)。 用select的优势在于它
可以同时处理多个connection。如果处理的连接数不是很高的话，使用select/epoll的web server不一定比使用multi-threading + blocking I/O
的web server性能更好，可能延迟还更大。select/epoll的优势并不是对于单个连接能处理得更快，而是在于能处理更多的连接。
在I/O multiplexing Model中，实际中对于每一个socket，一般都设置成为non-blocking，但是，process其实是一直被block的。
只不过process是被select这个函数block，而不是被socket I/O给block。

## select
select函数是I/O多路复用的函数，它主要的功能是用来等文件描述符中的事件是否就绪，select可以使我们在同时等待多个文件缓冲区，减少I/O等待的时间，
能够提高进程的IO效率。
```
#include <sys/select.h>

int select(
    int             nfds, 
    fd_set*         readfds,             
    fd_set*         writefds,
    fd_set*         exceptfds,
    struct timeval* timeout);
```
## 参数
监听小于`FD_SIZE(1024)`的文件描述符:
* `nfds`：监听(3个集合中)fd的最大值加1，如监听的fd为2、5、9，则应传10
* `readfds`：关注读的fd集合，select返回后，除了就绪的fd，集合中其余的fd都会被清除
* `writefds`：关注写的fd集合，select返回后，除了就绪的fd，集合中其余的fd都会被清除
* `exceptfds`：关注发生异常的fd，select返回后，除了就绪的fd，集合中其余的fd都会被清除
* `timeout`：设置select的阻塞时间，若不阻塞，则设置为NULL。若阻塞，可以设置为`struct timeval time = {5, 0};`

## 返回值
* 出错返回`-1`
* 超时返回`0`
* 成功返回发生变化的fd总数

## 相关宏
* `FD_ZERO(fd_set* fdset)`
* `FD_SET(int fd, fd_set* fdset)`
* `FD_CLR(int fd, fd_set* fdset)`
* `FD_ISSET(int fd, fd_set* fdset)`
```
fd_set set;
FD_ZERO(&set);    // 0 0 0 0 0 ... 
FD_SET(1, &set);  // 0 1 0 0 0 ...
FD_SET(3, &set);  // 0 1 0 1 0 ...
FD_CLR(1, &set);  // 0 0 0 1 0 ...
```


