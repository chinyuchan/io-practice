# Nonblock Multi-thread

可以通过非阻塞IO来解决accept阻塞的问题。即对sock_fd做如下设置：
```
fcntl(sock_fd, F_SETFL, O_NONBLOCK);
```
运行效果如下：

![nonblock](../pics/nonblock-multi.png)


