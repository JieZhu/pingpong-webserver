#ifndef SOCKET_NODE_H_
#define SOCKET_NODE_H_

#include <netinet/in.h>

/* A linked list node data structure to maintain application
 information related to a connected socket */
struct node {
  int socket;
  struct sockaddr_in client_addr;
  int pending_data; /* flag to indicate whether there is more data to send */
  /* you will need to introduce some variables here to record
   all the information regarding this socket.
   e.g. what data needs to be sent next */

  /* each node should have its own receiving buffer in
  case a shared receiving buffer will be overwritten when
  there are more than one client connecting to the server*/
  char* buf;
  int nbytes;
  int BUF_LEN;
  struct node* next;
};

/* remove the data structure associated with a connected socket
 used when tearing down the connection */
void dump(struct node*, int);

/* create the data structure associated with a connected socket */
void add(struct node*, int, struct sockaddr_in);

#endif
