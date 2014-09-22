#include "web_mode.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

_Bool not_exit(char* arguments) {
    struct stat dir_info;
    return (stat(arguments, &dir_info) == -1);
}
void wrong_req(int sock) {
    /*char* error_head = "HTTP/1.0 501 Not Implemented\r\n"; //construct 501 error header
    int len = strlen(error_head);
    if (sendall(sock, error_head, &len) == -1) {   //send to client
        printf("Sending failed!");
        return;
    }
    char* error_type = "Content-type: text/plain\r\n";
    len = strlen(error_type);
    if (sendall(sock, error_type, &len) == -1) {
        printf("Sending failed!");
        return;
    }
    char* error_end = "\r\n";
    len = strlen(error_end);
    if (sendall(sock, error_end, &len) == -1) {
        printf("Sending failed!");
        return;
    }
    char* prompt_info = "The command is not yet completed\r\n";
    len = strlen(prompt_info);
    if (sendall(sock, prompt_info, &len) == -1) {
        printf("Sending failed!");
        return;
    }*/
}

void file_not_found(char* arguments, int sock) {
    /*char* error_head = "HTTP/1.0 404 Not Found\r\n";   //construct 404 error header
    int len = strlen(error_head);
    if (sendall(sock, error_head, &len) == -1) {    //send to client
        printf("Sending error!");
        return;
    }
    char* error_type = "Content-type: text/plain\r\n";
    len = strlen(error_type);
    if (sendall(sock, error_type, &len) == -1) {
        printf("Sending error!");
        return;
    }
    char* error_end = "\r\n";
    len = strlen(error_end);
    if (sendall(sock, error_end, &len) == -1) {
        printf("Sending error!");
        return;
    }
    char prompt_info[50] = "Not found:  ";
    strcat(prompt_info, arguments);
    len = strlen(prompt_info);
    if (sendall(sock, prompt_info, &len) == -1) {
        printf("Sending error!");
        return;
    }*/
}

char* file_type(char* arg) {
    /*char * temp;
    if ((temp=strrchr(arg,'.')) != NULL) {
        return temp+1;
    }*/
    return "";
}

void send_header(int send_to, char* content_type) {

    /*char* head = "HTTP/1.0 200 OK\r\n";     //normal header information
    int len = strlen(head);
    if (sendall(send_to, head, &len) == -1) {   //send to the client
        printf("Sending error");
        return;
    }
    if (content_type) {         //content_type is not null
        char temp_1[30] = "Content-type: ";    //prepare the string
        strcat(temp_1, content_type);     //make content_type
        strcat(temp_1, "\r\n");
        len = strlen(temp_1);
        if (sendall(send_to, temp_1, &len) == -1) {
            printf("Sending error!");
            return;
        }
    }*/
}



void send_file(char* arguments, int sock) {
    /*char* extension = file_type(arguments);    //get the extension of the file
    char* content_type = "text/plain";     //initialization type='text/plain'
    FILE* read_from;         //read file
    int readed = -1;         //bytes per read

    if (strcmp(extension, "html") == 0) {
        content_type = "text/html";
    }
    read_from = fopen(arguments, "r");     //open the file for reading
    if(read_from != NULL) {
        char read_buf[128];
        send_header(sock, content_type);
        send(sock, "\r\n", 2, 0);
        while(!feof(read_from)) {      //whether is finished
            fgets(read_buf, 128, read_from);   //read the file
            int len = strlen(read_buf);
            if (sendall(sock, read_buf, &len) == -1) { //send data, if failed, go on sending
                printf("Sending error!");
                continue;
            }
        }
    }*/
}

void handle_req(char* request, int client_sock) {
    /*char command[BUFSIZ];        //store the command, GET PUT
    char arguments[BUFSIZ];        //stroe the file


    strcpy(arguments, "./");       //add as a path


    if (sscanf(request, "%s%s", command, arguments+2) != 2) {
        return;
    }

    printf("handle_cmd:    %s\n",command);    //print the command
    printf("handle_path:   %s\n",arguments);   //print the path

    if (strcmp(command, "GET") != 0) {     //check the command type
        wrong_req(client_sock);
        return;
    }
    if (not_exit(arguments)) {       //check the file's existence
        file_not_found(arguments, client_sock);
        return;
    }
    send_file(arguments, client_sock);     //send to client

    return;*/
}
