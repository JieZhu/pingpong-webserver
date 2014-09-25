#include "web_mode.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int sendall(int s, char *buf, int *len) {
    int total = 0;           // 已经发送字节数
    int bytesleft = *len;                                   //还剩余多少字节
    int n;
    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }
    *len = total;           // 返回实际发送出去的字节数
    return n==-1?-1:0;          // 成功发送返回0 失败-1
}

_Bool not_exit(char* arguments) {
    struct stat dir_info;
    return (stat(arguments, &dir_info) == -1);
}

_Bool is_dir(char* dir){
    struct stat info;
    stat(dir, &info);
    if(S_ISDIR(info.st_mode))
        return 1;
    return 0;
}

void wrong_req(int sock) {
    char* error_head = "HTTP/1.1 501 Not Implemented\r\n"; //construct 501 error header
    int len = strlen(error_head);
    if (sendall(sock, error_head, &len) == -1) {   //send to client
        printf("Sending failed!");
        internal_server_error(error_head, sock);
        return;
    }
    char* error_type = "Content-type: text/plain\r\n";
    len = strlen(error_type);
    if (sendall(sock, error_type, &len) == -1) {
        printf("Sending failed!");
        internal_server_error(error_head, sock);
        return;
    }
    char* error_end = "\r\n";
    len = strlen(error_end);
    if (sendall(sock, error_end, &len) == -1) {
        printf("Sending failed!");
        internal_server_error(error_head, sock);
        return;
    }
    char* prompt_info = "<html><head></head><body>The command is not yet completed\r\n<br/></body></html>";
    len = strlen(prompt_info);
    if (sendall(sock, prompt_info, &len) == -1) {
        printf("Sending failed!");
        internal_server_error(error_head, sock);
        return;
    }
}

void internal_server_error(char* dir, int client_sock){
    char* error_head = "HTTP/1.1 500 Internal Server Error\r\n";
    int len = strlen(error_head);
    if (sendall(client_sock, error_head, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
    char* error_type = "Content-type: text/html\r\n";
    len = strlen(error_type);
    if (sendall(client_sock, error_type, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
    char* error_end = "\r\n";
    len = strlen(error_end);
    if (sendall(client_sock, error_end, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
    char* prompt_info = "<html><head></head><body>400 Internal Server Error<br/></body></html>";
    
    len = strlen(prompt_info);
    if (sendall(client_sock, prompt_info, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
}

void bad_request(char* dir, int client_sock){
    char* error_head = "HTTP/1.1 400 Bad Request\r\n";   //construct 404 error header
    int len = strlen(error_head);
    if (sendall(client_sock, error_head, &len) == -1) {    //send to client
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
    char* error_type = "Content-type: text/html\r\n";
    len = strlen(error_type);
    if (sendall(client_sock, error_type, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
    char* error_end = "\r\n";
    len = strlen(error_end);
    if (sendall(client_sock, error_end, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
    char* prompt_info = "<html><head></head><body>400 Bad Requset<br/></body></html>";
    
    len = strlen(prompt_info);
    if (sendall(client_sock, prompt_info, &len) == -1) {
        printf("Sending error!");
        internal_server_error(dir, client_sock);
        return;
    }
}

void file_not_found(char* arguments, int sock) {
    char* error_head = "HTTP/1.1 404 Not Found\r\n";   //construct 404 error header
    int len = strlen(error_head);
    if (sendall(sock, error_head, &len) == -1) {    //send to client
        printf("Sending error!");
        internal_server_error(arguments, sock);
        return;
    }
    char* error_type = "Content-type: text/html\r\n";
    len = strlen(error_type);
    if (sendall(sock, error_type, &len) == -1) {
        printf("Sending error!");
        internal_server_error(arguments, sock);
        return;
    }
    char* error_end = "\r\n";
    len = strlen(error_end);
    if (sendall(sock, error_end, &len) == -1) {
        printf("Sending error!");
        internal_server_error(arguments, sock);
        return;
    }
    char* prompt_info = "<html><head></head><body>404 File not found<br/></body></html>";

    
    len = strlen(prompt_info);
    if (sendall(sock, prompt_info, &len) == -1) {
        printf("Sending error!");
        internal_server_error(arguments, sock);
        return;
    }
}

char* file_type(char* arg) {
    char * temp;
    if ((temp=strrchr(arg,'.')) != NULL) {
        return temp+1;
    }
    return "";
}

void send_header(int send_to, char* content_type) {

    char* head = "HTTP/1.1 200 OK\r\n";     //normal header information
    int len = strlen(head);
    if (sendall(send_to, head, &len) == -1) {   //send to the client
        printf("Sending error");
        internal_server_error(head, send_to);
        return;
    }
    if (content_type) {         //content_type is not null
        char temp_1[30] = "Content-type: ";    //prepare the string
        strcat(temp_1, content_type);     //make content_type
        strcat(temp_1, "\r\n");
        len = strlen(temp_1);
        if (sendall(send_to, temp_1, &len) == -1) {
            printf("Sending error!");
            internal_server_error(head, send_to);
            return;
        }
    }
}



void send_file(char* arguments, int sock) {
    char* extension = file_type(arguments);    //get the extension of the file
    char* content_type = "text/html";     //initialization type='text/plain'
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
                internal_server_error(arguments, sock);
                continue;
            }
        }
    }
}

void handle_req(char* request, int client_sock, char* root_dir) {
    char command[BUFSIZ];        //store the command, GET PUT
    char arguments[BUFSIZ];        //stroe the file
    strcpy(arguments, "");       //add as a path

    if (sscanf(request, "%s%s", command, arguments) != 2) {
        return;
    }

    char dir[BUFSIZ];
    sprintf(dir, "%s%s", root_dir, arguments);
    
    if (strcmp(command, "GET") != 0) {     //check the command type
        wrong_req(client_sock);
        return;
    }
    
    if(strrchr(dir, "../")){
        bad_request(dir, client_sock);
        printf("Bad request ../\n");
        return;
    }
    
    if (is_dir(dir)) {
        bad_request(dir, client_sock);
        printf("Bad request\n");
        return;
    }
    if (not_exit(dir)) {       //check the file's existence
        file_not_found(dir, client_sock);
        printf("file not found\n");
        return;
    }
    send_file(dir, client_sock);     //send to client
    printf("file sent");
    return;
}


