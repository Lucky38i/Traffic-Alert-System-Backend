//
// Created by almcb on 08/11/2019.
//

#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>



#define PORT 8080
#define BACKLOG 1

int main () {
    int server_fd, new_socket; long valread;
    struct sockaddr_in address = {};
    int addrlen = sizeof(address);

    char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0 )) < 0) {
        perror("cannot create socket");
        return 0;
    }



    /* htonl converts a long integer (e.g. address) to a network representation */
    /* htons converts a short integer (e.g. port) to a network representation */


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // Assign transport address to socket
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        return 0;
    }

    // Tells socket to accept incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        printf("Waiting for new connection");

        if ((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen))< 0){
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));
        printf("Hello message sent");
        close(new_socket);
    }
}
