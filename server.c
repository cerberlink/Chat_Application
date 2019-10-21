#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    char server_message[256] = "You've missed a server call";
    //Create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //define the server address and create
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3490);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //calling bind function to get specified IP and port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    //starting accept function
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    send(client_socket, server_message, sizeof(server_message), 0);

    //close the socket
    close(server_socket);
    return 0;
}