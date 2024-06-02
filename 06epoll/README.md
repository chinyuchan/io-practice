# epoll
`epoll`是Linux内核为处理大批量文件描述符而作了改进的`poll`，是Linux下多路复用IO接口`select/poll`的增强版本，它能显著提高程序在大量并发连接中只有
少量活跃的情况下的系统CPU利用率。另一点原因就是获取事件的时候，它无须遍历整个被侦听的描述符集，只要遍历那些被内核IO事件异步唤醒而加入Ready队列
的描述符集合就行了。`epoll`除了提供`select/poll`那种IO事件的水平触发（Level Triggered）外，还提供了边缘触发（Edge Triggered），这就使得用户
空间程序有可能缓存I/O状态，减少`epoll_wait/epoll_pwait`的调用，提高应用程序效率。
```
#include <sys/epoll.h>

int epoll_create(int size);   // 2.6.8以后，size参数忽略了，但必须大于0
int epoll_create1(int flags); 
```
* 功能：创建`epoll`实例，就是一个文件描述符
* 参数：
  * `flags`: 如果`flags`为0，那就和`epoll_create`一样
* 返回值：成功返回一个文件描述符，出错返回-1
```
#include <sys/epoll.h>

int epoll_ctl(
    int                  epfd, 
    int                  op, 
    int                  fd, 
    struct epoll_event*  event
);
```
* 功能：将感兴趣的`fd`在`epfd`中进行添加、修改、删除
* 参数：
  * `epfd`：`epoll_create1`的返回值
  * `op`：相关操作
    * `EPOLL_CTL_ADD`：添加
    * `EPOLL_CTL_MOD`：修改
    * `EPOLL_CTL_DEL`：删除
  * `fd`：感兴趣的文件描述符
  * `event`：`struct epoll_event`结构体指针
* 返回值：成功返回0，出错返回-1
```
#include <sys/epoll.h>

int epoll_wait(
    int                  epfd, 
    struct epoll_event*  events,    
    int                  maxevents, 
    int                  timeout
);
```
* 功能：等待就绪的fd
* 参数
  * `epfd`：`epoll_create1`的返回值
  * `events`：一个`struct epoll_event`数组
  * `maxevnets`：最大监控的事件数
  * `timeout`：：阻塞时间，毫秒
* 返回值：成功返回就绪的文件描述符数量，出错返回-1

