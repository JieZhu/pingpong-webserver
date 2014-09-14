#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void set_timestamp(char* sendbuf, struct timeval* tv) {
  if (sendbuf && tv) {

    if (gettimeofday(tv, NULL) != 0) {
      perror("Failed to get current time!");
      abort();
    }
    *(int*)(sendbuf + 2) = (int)htonl(tv->tv_sec);
    *(int*)(sendbuf + 6) = (int)htonl(tv->tv_usec);
  }
}

void get_timestamp(char* recvbuf, struct timeval* tv) {
  if (recvbuf && tv) {
    tv->tv_sec = ntohl(*(int*)(recvbuf + 2));
    tv->tv_usec = ntohl(*(int*)(recvbuf + 6));
  }
}

unsigned long get_latency(struct timeval* start, struct timeval* end) {
  if (start && end) {
    unsigned long interval = 1000000 * (end->tv_sec - start->tv_sec) + end->tv_usec - start->tv_usec;

    return interval / 2;
  }

  return 0;
}
