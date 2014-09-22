#ifndef CLIENT_UTIL_H_
#define CLIENT_UTIL_H_

int client_ready(unsigned int, unsigned short);
void send_blocking(int, char*, int);
void recv_blocking(int, char*, int);

#endif
