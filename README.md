
## tcpcapture

`tcpcapture` 是一个简单的 tcp 监听工具，可以用来监听 tcp 包，或者监听 tcp 包中的数据。支持自定义插件，可以用来解析自定义的协议。

### 启动参数

```bash

Usage: ./tcpcapture [options] ...

  -h, --help    Print help and exit
  -V, --version    Print version and exit
  -d, --device=STRING    device name, default use first eth device
  -i, --ip=STRING    <ip address>
  -p, --port=STRING    <port>
  -P, --plugin=STRING    plugin shared library name

```

### 自定义插件开发

支持动态库插件，plugin 需要实现下面的接口，在动态库的实现接口里解析自定义的协议。`connection_data` 在头文件 `tcpcapture.h` 中定义。

```c

typedef struct __connection_data {
    /** Source IP address */
    int32_t src_ip;
    /** Destination IP address */
    int32_t dst_ip;
    /** Source TCP port */
    uint16_t src_port;
    /** Destination TCP port */
    uint16_t dst_port;
    /** A 4-byte hash key representing the connection */
    uint32_t flowKey;
    /** Start TimeStamp of the connection */
    timeval startTime;
    /** End TimeStamp of the connection */
    timeval endTime;
} connection_data;

void init();
void connection_start(connection_data* key);
void connection_send(connection_data* key, const char* buffer, int len);
void connection_recv(connection_data* key, const char* buffer, int len);
void connection_close(connection_data* key);
void close();
```