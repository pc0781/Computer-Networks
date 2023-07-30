#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    int addr_size;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    // Create a socket for the server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Error creating socket");
        WSACleanup();
        return 1;
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Error binding socket");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        perror("Error listening");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening for incoming connections...\n");

    // Accept a client connection
    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    if (client_socket == INVALID_SOCKET) {
        perror("Error accepting connection");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    // TCP Handshake (Server side)
    // Step 1: Receive SYN from the client
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received SYN from client.\n");

    // Step 2: Send SYN-ACK to the client
    send(client_socket, "SYN-ACK", strlen("SYN-ACK"), 0);
    printf("Sent SYN-ACK to client.\n");

    // Step 3: Receive ACK from the client
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received ACK from client. TCP Handshake completed.\n");

    // Data transfer can happen now
    // Add your data transfer logic here...

    // Close the sockets
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
