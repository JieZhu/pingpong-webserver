#ifndef WEB_MODE_H_
#define WEB_MODE_H_

#include "server_util.h"

_Bool not_exit(char*);
void wrong_req(int);
void file_not_found(char*, int);
void internal_server_error(char* dir, int client_sock);
char* file_type(char*);
void send_header(int, char*);
void send_file(char*, int);
void handle_req(char*, int, char*);

#endif
