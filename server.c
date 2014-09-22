#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "socket_node.h"
#include "server_util.h"
#include "web_mode.h"

#define EOL "\r\n"
#define EOL_SIZE 2

int main(int argc, char **argv) {
  if(argc != 4 && argc != 2){
    fprintf(stderr, "The number of arguments is wrong!");
    abort();
  }

  /* client socket address variables */
  struct sockaddr_in addr;
  unsigned short server_port = atoi(argv[1]);
  if (server_port < 18000 || server_port > 18200) {
    fprintf(stderr, "Argument port should be in the range of [18000, 18200]!");
    abort();
  }

  /* set argument for www mode */
  char* mode = NULL;
  char* root_dir = NULL;
  if (argc == 4) {
    mode = argv[2];
    if (strcmp(mode, "www") == 0) {
      root_dir = argv[3];
      printf("%s", mode);
      fprintf(stdout, "web server started ...\n");
    }
  }

  /* socket and option variables */
  int sockfd, new_sockfd, max;

  /* socket address variables for a connected client */
  socklen_t addr_len = sizeof(struct sockaddr_in);

  /* maximum number of pending connection requestes */
  int BACKLOG = 5;

  /* variables for select */
  fd_set read_set, write_set;
  struct timeval time_out;
  int select_retval;

  /* linked list for keeping track of connected sockets */
  struct node head;
  struct node* current;
  struct node* next;

  /* a buffer to read data */
  char* recvbuf;
  int BUF_LEN = 65535;//the maxium size in byte
  recvbuf = (char *)malloc(BUF_LEN);
  if (!recvbuf) {
    perror("failed to allocate receiving buffer");
    abort();
  }

  /* initialize dummy head node of linked list */
  head.socket = -1;
  head.next = 0;

  /* binding & listening */
  sockfd = server_ready(server_port, BACKLOG);

  /* now we keep waiting for incoming connections,
   check for incoming data to receive,
   check for ready socket to send more data */
  while (1){

    /* set up the file descriptor bit map that select should be watching */
    FD_ZERO (&read_set); /* clear everything */
    FD_ZERO (&write_set); /* clear everything */

    FD_SET (sockfd, &read_set); /* put the listening socket in */
    max = sockfd; /* initialize max */

    /* put connected sockets into the read and write sets to monitor them */
    for (current = head.next; current; current = current->next) {
      FD_SET(current->socket, &read_set);

      if (current->pending_data) {
        /* there is data pending to be sent, monitor the socket
         in the write set so we know when it is ready to take more
         data */
        FD_SET(current->socket, &write_set);
      }

      if (current->socket > max) {
        /* update max if necessary */
        max = current->socket;
      }
    }

    time_out.tv_usec = 100000; /* 1-tenth of a second timeout */
    time_out.tv_sec = 0;

    /* invoke select, make sure to pass max+1 !!! */
    select_retval = select(max + 1, &read_set, &write_set, NULL, &time_out);
    if (select_retval < 0) {
      perror ("select failed");
      abort ();
    }

    if (select_retval == 0) {
      /* no descriptor ready, timeout happened */
      continue;
    }

    /* at least one file descriptor is ready */
    if (select_retval > 0) {
      /* check the server socket */
      if (FD_ISSET(sockfd, &read_set)) {
        /* there is an incoming connection, try to accept it */
        new_sockfd = accept (sockfd, (struct sockaddr *)&addr, &addr_len);

        if (new_sockfd < 0) {
          perror ("error accepting connection");
          abort ();
        }

        if (root_dir != NULL) {

        }

        /* make the socket non-blocking so send and recv will
         return immediately if the socket is not ready.
         this is important to ensure the server does not get
         stuck when trying to send data to a socket that
         has too much data to send already.
         */
        if (fcntl (new_sockfd, F_SETFL, O_NONBLOCK) < 0) {
          perror ("making socket non-blocking");
          abort ();
        }

        /* the connection is made, everything is ready */
        /* let's see who's connecting to us */
        fprintf(stdout, "Accepted connection. Client IP address is: %s\n",
               inet_ntoa(addr.sin_addr));

        /* remember this client connection in our linked list */
        add(&head, new_sockfd, addr);
      }

      /* check other connected sockets, see if there is
       anything to read or some socket is ready to send
       more pending data */
      for (current = head.next; current; current = next) {
        next = current->next;
        /* number of bytes sent/received */
        int count;

        /* see if we can now do some previously unsuccessful writes */
        if (FD_ISSET(current->socket, &write_set)) {
          count = send_non_blocking(current);
          handle_error(count, &head, current);
        }

        if (FD_ISSET(current->socket, &read_set)) {
          count = recv_non_blocking(current);
          handle_error(count, &head, current);
        }
      }
    }
  }
}
