#include "library.h"


int plugin_init() {
    return 0;
}

void plugin_connection_start(connection_data *key) {

}

void plugin_connection_send(connection_data *key, const char *buffer, int len) {

}

void plugin_connection_recv(connection_data *key, const char *buffer, int len) {

}

void plugin_connection_close(connection_data *key) {

}

void plugin_close() {

}
