#include "server_util.h"
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int server_ready(unsigned short server_port, int BACKLOG) {
  struct sockaddr_in sin;       //server address struct
  int sockfd;
  int optval = 1;

  if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("Failed to open TCP socket!");
    abort();
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    perror("setting TCP socket option");
    abort();
  }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(server_port);

  if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    perror("binding socket to address");
    abort();
  }

  if (listen(sockfd, BACKLOG) < 0) {
    perror("listen on socket failed");
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

int send_non_blocking(struct node* current) {
  int count = send(current->socket,
                   current->buf + current->nbytes,
                   current->BUF_LEN - current->nbytes,
                   MSG_DONTWAIT);
  if (count <= 0) {
    return count;
  }

  current->nbytes = count;
  if (current->nbytes == current->BUF_LEN) {
    current->pending_data = 0;
    current->nbytes = 0;
    current->BUF_LEN = 0;
  }

  return count;
}

int recv_non_blocking(struct node* current) {
  int count;
  if (current->BUF_LEN == 0) {
    count = recv(current->socket,
                 current->buf,
                 65535, MSG_DONTWAIT);
  } else {
    count = recv(current->socket,
                 current->buf + current->nbytes,
                 65535 - current->nbytes, MSG_DONTWAIT);
  }

  if (count <= 0) {
    return count;
  }

  current->nbytes += count;
  if (current->BUF_LEN == 0) {
    current->BUF_LEN = ntohs(*(unsigned short*)current->buf);
  }

  if (current->nbytes == current->BUF_LEN) {
    current->pending_data = 1;
    current->nbytes = 0;
  }

  return count;
}

void handle_error(int count, struct node* head, struct node* current) {
  if (count <= 0) {
    /* something is wrong */
    if (count == 0) {
      fprintf(stdout, "Client closed connection. Client IP address is: %s\n",
              inet_ntoa(current->client_addr.sin_addr));
    } else {
      perror("error receiving from a client");
    }

    /* connection is closed, clean up */
    dump(head, current->socket);
  }
}
