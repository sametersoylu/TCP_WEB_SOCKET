#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "myConlib.h"

int main(int argc, char **argv) {
    ClientConConfig myConfig = init_client(7000);
    ConnectServer(&myConfig);
    char data[1024];
    recv(myConfig.sock, data, 1024, 0);
    strprintln(data);
    
    char msg[1024];
    while(strcmp(msg, "exit")) {
        int i; 
        write(1, "Send: ", 7);
        scanf("%1024s", &msg);
        send(myConfig.sock, msg, strlen(msg), 0); 
        int size = recv(myConfig.sock, data, 1024, 0); 
        write(1, "Recieved: ", 11);
        write(1, data, size);
        write(1, "\n", 1);
    }
    return 0; 

}