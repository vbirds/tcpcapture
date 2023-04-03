#pragma once

#include "tcpcapture.h"


class Plugin
{
public:
    Plugin();
    ~Plugin();

    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;

    int load(const char* file);
    void unload();

public:
    int  plugin_init();
    void plugin_connection_start(connection_data* key);
    void plugin_connection_send(connection_data* key, const char* buffer, int len);
    void plugin_connection_recv(connection_data* key, const char* buffer, int len);
    void plugin_connection_close(connection_data* key);
    void plugin_close();

private:
    plugin_init_hook m_init;
    plugin_connection_start_hook m_connection_start_hook;
    plugin_connection_send_hook m_connection_send_hook;
    plugin_connection_recv_hook m_connection_recv_hook;
    plugin_connection_close_hook m_connection_close_hook;
    plugin_close_hook m_close;
};
