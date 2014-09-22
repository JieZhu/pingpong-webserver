#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "client_util.h"
#include "time.h"

int main(int argc, char** argv) {
  if (argc != 5) {
    fprintf(stderr, "The number of arguments is wrong!");
    abort();
  }

  /* the host where the server is running */
  struct hostent* host = gethostbyname(argv[1]);
  unsigned int server_addr = *(unsigned int*)host->h_addr_list[0];

  /* the port on which the server is running */
  unsigned short server_port = atoi(argv[2]);
  if (server_port < 18000 || server_port > 18200) {
    fprintf(stderr, "Argument port should be in the range of [18000, 18200]!");
    abort();
  }

  /* the size in bytes of each message to send */
  unsigned short size = atoi(argv[3]);
  if (size < 10 || size > 65535) {
    fprintf(stderr, "Argument size should be in the range of [10, 65535]!");
    abort();
  }

  /* the number of message exchanges to perform */
  unsigned short count = atoi(argv[4]);
  if (count < 1 || count > 10000) {
    fprintf(stderr, "Argument count should be in the range of [1, 10000]!");
    abort();
  }

  /* buffer for receiving message */
  char* recvbuf = (char*)malloc(size);
  if (!recvbuf) {
    perror("Failed to allocate recvbuffer!");
    abort();
  }

  /* buffer for sending message */
  char* sendbuf = (char*)malloc(size);
  if (!sendbuf) {
    perror("Failed to allocate sendbuffer!");
    abort();
  }

  /* the client socket */
  int sockfd = client_ready(server_addr, server_port);

  /* calculate the average latency */
  unsigned long sum_latency = 0;
  *(short*)sendbuf = (short)htons(size);
  struct timeval start, end;
  int test_count = count;

  while (test_count > 0) {
    set_timestamp(sendbuf, &start);
    send_blocking(sockfd, sendbuf, size);

    recv_blocking(sockfd, recvbuf, size);
    get_timestamp(&end);

    sum_latency += get_latency(&start, &end);
    --test_count;
  }

  /* latency is expressed in millisecond */
  printf("Average latency: %.3f ms\n", (double)sum_latency / (1000 * count));

  /* free resources */
  close(sockfd);
  free(recvbuf);
  free(sendbuf);

  return 0;
}
