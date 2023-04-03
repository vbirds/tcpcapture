
#pragma once

#include <stdint.h>
#include <time.h>

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


#define DYLIB_PLUGIN_INIT_SYMBOL "init"
#define DYLIB_PLUGIN_CONNECTION_START_HOOK_SYMBOL "connection_start"
#define DYLIB_PLUGIN_CONNECTION_SEND_HOOK_SYMBOL "connection_send"
#define DYLIB_PLUGIN_CONNECTION_RECV_HOOK_SYMBOL "connection_recv"
#define DYLIB_PLUGIN_CONNECTION_CLOSE_HOOK_SYMBOL "connection_close"
#define DYLIB_PLUGIN_CLOSE_SYMBOL "close"

typedef void (*plugin_init_hook)();
typedef void (*plugin_connection_start_hook)(connection_data* key);
typedef void (*plugin_connection_send_hook)(connection_data* key, const char* buffer, int len);
typedef void (*plugin_connection_recv_hook)(connection_data* key, const char* buffer, int len);
typedef void (*plugin_connection_close_hook)(connection_data* key);
typedef void (*plugin_close_hook)();


#ifdef __cplusplus
};
#endif
