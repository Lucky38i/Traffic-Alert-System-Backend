//
// Created by almcb on 08/11/2019.
//

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "lib/cJSON.h"

#define PORT 8080
#define BACKLOG 1

typedef struct {
    int ID;
    char vehicle[50];

} Alert;


// TODO Receive JSON Object
int main () {
    int server_fd, new_socket; long valread;
    struct sockaddr_in address = {};
    int addrlen = sizeof(address);
    Alert test;
    cJSON *ID;
    cJSON *vehicle;

    char *hello = "Hello from C server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0 )) < 0) {
        perror("cannot create socket");
        return 0;
    }
    printf("%s \n","Socket Created");


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
    printf("%s \n","Socket Bound");

    // Tells socket to accept incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    printf("%s \n","Socket Listening");

    while (1) {
        printf("%s \n","Waiting for new connection");

        if ((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen))< 0){
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        printf("Connected!\n");


        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("valread: %ld, buffer: %s\n", valread, buffer);


        cJSON *json = cJSON_Parse(buffer);
        test.ID =cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
        strcpy(test.vehicle, cJSON_GetObjectItemCaseSensitive(json, "vehicle")->valuestring);
        printf("The Test ID is: %i The Test Vehicle is: %s\n", test.ID, test.vehicle);

        write(new_socket, hello, strlen(hello));
        printf("%s\n","Hello message sent");
        //cJSON_Delete(json);
        close(new_socket);
    }
}
