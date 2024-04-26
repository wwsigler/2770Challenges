#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[1024] = {0};
    char method[16] = {0};
    char url[1024] = {0};
    char protocol[16] = {0};
    char response[2048] = {0}; // This should be large enough to hold the response

    // Read the request into the buffer
    read(sock, buffer, sizeof(buffer));

    // Use sscanf to parse the method, URL, and protocol from the request
    sscanf(buffer, "%15s %1023s %15s", method, url, protocol); // Limit the input to prevent buffer overflow

    // Construct the response
    int content_length = strlen("Hello, world!"); // The content we want to send back
    sprintf(response, 
            "HTTP/1.1 200 OK\n"
            "Content-Type: text/html\n"
            "Content-Length: %d\n\n" // The length of the content
            "Method: %s\n"
            "URL: %s\n"
            "Protocol: %s\n"
            "Hello, world!\n\n", // The content
            content_length, method, url, protocol);

    // Send the response
    write(sock, response, strlen(response));

    printf("Response sent\n");
    close(sock);
    free(socket_desc);
    return NULL;
}


int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        pthread_t thread;
        new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        
        if (pthread_create(&thread, NULL, connection_handler, (void*)new_sock) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
        
        // Detach the thread so it doesn't need to be joined
        pthread_detach(thread);
    }
    
    return 0;
}
