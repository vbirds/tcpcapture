
#include "plugin.h"
#include "dynamiclib_mgr.h"

#include <iostream>

void default_plugin_init() {
    std::cout << "default_plugin_init" << std::endl;
}

void default_plugin_connection_start(connection_data *key) {
    std::cout << "default_plugin_connection_start" << std::endl;
}

void default_plugin_connection_send(connection_data *key, const char *buffer, int len) {
    std::cout << "default_plugin_connection_send" << std::endl;
}

void default_plugin_connection_recv(connection_data *key, const char *buffer, int len) {
    std::cout << "default_plugin_connection_recv" << std::endl;
}

void default_plugin_connection_close(connection_data *key) {
    std::cout << "default_plugin_connection_close" << std::endl;
}

void default_plugin_close() {
    std::cout << "default_plugin_close" << std::endl;
}


Plugin::Plugin() {
    m_init = default_plugin_init;
    m_connection_start_hook = default_plugin_connection_start;
    m_connection_send_hook = default_plugin_connection_send;
    m_connection_recv_hook = default_plugin_connection_recv;
    m_connection_close_hook = default_plugin_connection_close;
    m_close = default_plugin_close;
}

Plugin::~Plugin() {
    unload();
}

int Plugin::load(const char *file) {

    int ret = DynamicLibMgr::Instance()->LoadLib(file);
    if (ret != 0 ) {
        return -1;
    }

    m_init = (plugin_init_hook)DynamicLibMgr::Instance()->GetSymbol(DYLIB_PLUGIN_INIT_SYMBOL);
    if (m_init == nullptr) {
        printf("load %s symble not found\n", DYLIB_PLUGIN_INIT_SYMBOL);
        return -1;
    }
    m_connection_start_hook = (plugin_connection_start_hook)DynamicLibMgr::Instance()->GetSymbol(DYLIB_PLUGIN_CONNECTION_START_HOOK_SYMBOL);
    if (m_connection_start_hook == nullptr) {
        printf("load %s symble not found\n", DYLIB_PLUGIN_CONNECTION_START_HOOK_SYMBOL);
        return -1;
    }
    m_connection_send_hook = (plugin_connection_send_hook)DynamicLibMgr::Instance()->GetSymbol(DYLIB_PLUGIN_CONNECTION_SEND_HOOK_SYMBOL);
    if (m_connection_send_hook == nullptr) {
        printf("load %s symble not found\n", DYLIB_PLUGIN_CONNECTION_SEND_HOOK_SYMBOL);
        return -1;
    }

    m_connection_recv_hook = (plugin_connection_recv_hook)DynamicLibMgr::Instance()->GetSymbol(DYLIB_PLUGIN_CONNECTION_RECV_HOOK_SYMBOL);
    if (m_connection_recv_hook == nullptr) {
        printf("load %s symble not found\n", DYLIB_PLUGIN_CONNECTION_RECV_HOOK_SYMBOL);
        return -1;
    }

    m_connection_close_hook = (plugin_connection_close_hook)DynamicLibMgr::Instance()->GetSymbol(DYLIB_PLUGIN_CONNECTION_CLOSE_HOOK_SYMBOL);
    if (m_connection_close_hook == nullptr) {
        printf("load %s symble not found\n", DYLIB_PLUGIN_CONNECTION_CLOSE_HOOK_SYMBOL);
        return -1;
    }
    m_close = (plugin_close_hook)DynamicLibMgr::Instance()->GetSymbol(DYLIB_PLUGIN_CLOSE_SYMBOL);
    if (m_close == nullptr) {
        printf("load %s symble not found\n", DYLIB_PLUGIN_CLOSE_SYMBOL);
        return -1;
    }

    return 0;
}

void Plugin::unload() {
    m_init = default_plugin_init;
    m_connection_start_hook = default_plugin_connection_start;
    m_connection_send_hook = default_plugin_connection_send;
    m_connection_recv_hook = default_plugin_connection_recv;
    m_connection_close_hook = default_plugin_connection_close;
    m_close = default_plugin_close;
}

int Plugin::plugin_init() {
    m_init();
    return 0;
}

void Plugin::plugin_connection_start(connection_data *key) {
    m_connection_start_hook(key);
}

void Plugin::plugin_connection_send(connection_data *key, const char *buffer, int len) {
    m_connection_send_hook(key, buffer, len);
}

void Plugin::plugin_connection_recv(connection_data *key, const char *buffer, int len) {
    m_connection_recv_hook(key, buffer, len);
}

void Plugin::plugin_connection_close(connection_data *key) {
    m_connection_close_hook(key);
}

void Plugin::plugin_close() {
    m_close();
}

