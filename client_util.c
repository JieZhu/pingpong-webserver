#include "client_util.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int client_ready(unsigned int server_addr, unsigned short server_port) {
  struct sockaddr_in sin;
  int sockfd;

  if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("Failed to open TCP socket!");
    abort();
  }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = server_addr;
  sin.sin_port = htons(server_port);

  if (connect(sockfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
    perror("Failed to connect to server!");
    abort();
  }

  return sockfd;
}

void send_blocking(int sockfd, char* buf, int len) {
  if (buf) {
    int nbytes, count;

    nbytes = 0;
    while (nbytes < len) {
      if ((count = send(sockfd, buf + nbytes, len - nbytes, 0)) < 0) {
        perror("failed to send data\n");
        abort();
      }

      nbytes += count;
    }
  }
}

void recv_blocking(int sockfd, char* buf, int size) {
  if (buf) {
    int nbytes, count;

    nbytes = 0;
    while (nbytes < size) {
      if ((count = recv(sockfd, buf + nbytes, size - nbytes, 0)) < 0) {
        perror("failed to receive data\n");
        abort();
      }

      nbytes += count;
    }
  }
}
