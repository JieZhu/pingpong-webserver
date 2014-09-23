#ifndef SERVER_UTIL_H_
#define SERVER_UTIL_H_

#include "socket_node.h"

int server_ready(unsigned short, int);
void send_blocking(int, char*, int);
void recv_blocking(int, char*, int);
int send_non_blocking(struct node*);
int recv_non_blocking(struct node*);
void handle_error(int, struct node*, struct node*);

#endif
