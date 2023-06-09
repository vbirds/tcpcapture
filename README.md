
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

支持动态库插件，在动态库的实现接口里解析自定义的协议。需要包含头文件 `plugin_def.h`，里面定义了动态库需要实现的接口，在cpp文件里实现这些接口，编译成动态库，然后在启动参数里指定动态库的名称即可。

```cpp
#pragma once

#include <stdint.h>
#include <time.h>
#include <cstddef>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#define API __declspec(dllexport)
#else
#define API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

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


API int plugin_init();
API void plugin_connection_start(connection_data* key);
API void plugin_connection_send(connection_data* key, const char* buffer, size_t len);
API void plugin_connection_recv(connection_data* key, const char* buffer, size_t len);
API void plugin_connection_close(connection_data* key);
API void plugin_close();

#ifdef __cplusplus
};
#endif
```