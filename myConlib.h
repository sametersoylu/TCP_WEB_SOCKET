#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* UTILIZATION FUNCTIONS */
//Print Error and close the socket
void printErr(const char * errTitle, int sock) {
    perror(errTitle); 
    close(sock);
    exit(1);
}
void printdash(const char * str, int length) {
    int i; 
    for(i = 0; i < length; i++) {
        write(1, str, 1);
    }
    write(1, "\n", 1);
}
//Print a string
void strprint(const char * str) {
    write(1, str, strlen(str));
}
//Print a string and add a new line after the string
void strprintln(const char * str) {
    write(1, str, strlen(str));
    write(1, "\n", 1); 
}
//Send data || if all data couldn't send at once, it'll continue to send from where it left
void sendall(int fd, const char * data, int size, int opt) {
    int sent = 0; 
    while(sent < size) {
        sent += send(fd, data+sent, size-sent, 0);
    }
}
//Auto sized string sender
void sendstring(int fd, const char * data, int opt) {
    sendall(fd, data, strlen(data), opt);
}
/* UTILIZATION FUNCTIONS */

//Type definitions for easier writing
typedef struct sockaddr_in sockaddr_in; 
typedef struct sockaddr* sockaddrptr; 

/* SERVER FUNCTIONS */
//Server Configs
struct ServerConnectionConfig {
    int sock, newsock, len;
    sockaddr_in server, remote; 
};


//Type definitions for easier writing
typedef struct ServerConnectionConfig ServConConfig; 


//Initialize server data
ServConConfig init_serv(int addr) {
    sockaddr_in server = {
        AF_INET, 
        htons(addr),
        INADDR_ANY
    }, remote;
    ServConConfig newConfig = {
        socket(AF_INET, SOCK_STREAM, 0),
        0,
        0,
        server, 
        remote
    };
    return newConfig; 
}
//Bind the server
void bindServer(ServConConfig * cnf, int addr) {
    if(bind(cnf->sock, (sockaddrptr)&cnf->server, sizeof(cnf->server))) {
        printErr("Bind", cnf->sock);
    }
}

//Accept clients and do something with in the function. 
void acceptClients(ServConConfig * cnf, void(func)(ServConConfig * cnf)) {
    while((cnf->newsock = accept(cnf->sock, (sockaddrptr)&cnf->remote, &cnf->len)) >= 0 ) {
        if(!fork()) {
            close(cnf->sock);
            strprintln("A client has successfully connected!");
            func(cnf);
            close(cnf->newsock);
            strprintln("Connection exited successfully!");
            exit(0);
        }
        
    }
}
/* SERVER FUNCTIONS */

/* CLIENT FUNCTIONS */
//Client Configs
struct ClientConnectionConfig {
    int sock;
    sockaddr_in server;
};

typedef struct ClientConnectionConfig ClientConConfig; 

ClientConConfig init_client(int addr) {
    sockaddr_in server = {
        AF_INET, 
        htons(addr),
        INADDR_ANY
    };
    ClientConConfig newConfig = {
        socket(AF_INET, SOCK_STREAM, 0),
        server
    };
    return newConfig; 
}

void ConnectServer(ClientConConfig * cnf) {
    if(connect(cnf->sock, (sockaddrptr)&cnf->server, sizeof(cnf->server))) {
        printErr("Connect", cnf->sock);
    } 
}

void recvnprint(ClientConConfig * cnf) {
    char data[1024]; 
    recv(cnf->sock, data, 1024, 0);
    strprintln(data);
}
/* CLIENT FUNCTIONS */