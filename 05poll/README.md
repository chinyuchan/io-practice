# poll

```
#include <poll.h>
int poll(struct pollfd* fds, nfds_t nfds, int timeout);
```
* `fds`：是一个`struct pollfd`数组，用于存放需要检测其状态的描述符；每当调用这个函数之后，系统不会清空这个数组
  * 与`select`不同，调用`select`之后，`select`会清空它所检测的描述符集合，导致每次调用`select`之前都必须把描述符重新加入到待检测的集合中
  * `select`适合于只检测一个描述符的情况，而`poll`适合于大量描述符的情况
* `nfds`：`nfds_t`类型的参数，用于标记数组fds中的结构体元素的总数量
* `timeout`：是poll函数调用阻塞的时间，单位毫秒


