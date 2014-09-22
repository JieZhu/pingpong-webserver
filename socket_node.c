#include "socket_node.h"
#include <stdlib.h>
#include <unistd.h>

void dump(struct node* head, int socket) {
  struct node *current, *temp;

  current = head;

  while (current->next) {
    if (current->next->socket == socket) {
      /* remove */
      temp = current->next;
      current->next = temp->next;
      close(temp->socket);
      free(temp->buf);
      free(temp); /* don't forget to free memory */
      return;
    } else {
      current = current->next;
    }
  }
}

void add(struct node* head, int socket, struct sockaddr_in addr) {
  struct node* new_node;

  new_node = (struct node *)malloc(sizeof(struct node));
  new_node->socket = socket;
  new_node->client_addr = addr;
  new_node->pending_data = 0;
  new_node->buf = (char *)malloc(65535);
  new_node->nbytes = 0;
  new_node->BUF_LEN = 0;
  new_node->next = head->next;
  head->next = new_node;
}
