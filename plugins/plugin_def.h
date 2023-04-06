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