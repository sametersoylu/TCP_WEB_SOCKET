#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "myConlib.h"

int * ptr;

void sigchld(int signal) {
    wait(NULL);
    strprintln("Child has exited successfully!");
    printdash("-", 15);
}

void sigint(int signal) {
    close(*ptr);
    strprintln("Program is terminating!");
}

void setsignals() {
    signal(SIGCHLD, &sigchld);
    signal(SIGINT, &sigint);
}

void myFoo(ServConConfig * cnf) {
    const char * initialmsg = "Hello, who are you?\n";
    send(cnf->newsock, initialmsg, 21, 0);
    printf("Sent: %s ", initialmsg);
    int size; 
    char data[1024]; 
    while((size = recv(cnf->newsock, data, 1024, 0)) > 0 && strcmp(data, "exit")) {
        write(1, "Recieved: ", 11);
        write(1, data, size);
        write(1, "\n", 1);
        printf("Size: %d\n", size);
        send(cnf->newsock, data, size, 0);
    }
}

int main(int argc, char **argv) {
    setsignals();
    ServConConfig myConfig = init_serv(7000);
    ptr = &myConfig.sock;
    bindServer(&myConfig, 7000);
    listen(myConfig.sock, 100); 
    printdash("-", 15);
    strprintln("Server successfully started!\nNow ready to accept connections!");
    strprintln("Waiting for a connection!");
    printdash("-", 15);
    acceptClients(&myConfig, &myFoo);
    return 0; 
}
