#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int fd = 0;
    char buff[1024];

    if (argc < 3)
    {
        printf("Less no of arguments !!");
        return 0;
    }

    //Setup Buffer Array
    memset(buff, '0', sizeof(buff));

    //Create Socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("Client Error: Socket not created succesfully");
        return 0;
    }

    //Structure to store details
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));

    //Initialize
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    int in = inet_pton(AF_INET, argv[1], &server.sin_addr);
    if (in < 0)
    {
        perror("Client Error: IP not initialized succesfully");
        return 0;
    }

    //Connect to given server
    in = connect(fd, (struct sockaddr *)&server, sizeof(server));
    if (in < 0)
    {
        perror("Client Error: Connection Failed.");
        return 0;
    }

    while (1)
    {
        printf("Please enter the message: ");
        bzero(buff, 256);
        fgets(buff, 255, stdin);

        printf("\nSending to SERVER: %s ", buff);

        /* Send message to the server */
        in = send(fd, buff, strlen(buff), 0);
        if (in < 0)
        {
            perror("\nClient Error: Writing to Server");
            return 0;
        }

        /* Now read server response */
        bzero(buff, 256);
        in = recv(fd, buff, 255, 0);
        if (in < 0)
        {
            perror("\nClient Error: Reading from Server");
            return 0;
        }
        printf("\nReceived FROM SERVER: %s ", buff);
    }
    close(fd);
    return 0;
}
// //int inet_pton(int af, const char *src, void *dst);
// struct sockaddr_in sa;                                          // IPv4
// struct sockaddr_in6 sa6;                                        // IPv6
// inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));             //IPv4
// inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); //IPv6

// struct addressinfo
// {
//     int ai_flag;                 // ai_passive, ai_canonname, etc
//     int ai_family;               // af_inet, af_inet6, af_unspec
//     int ai_socktype;             // sock_stream, sock_dgram
//     int ai_protocol;             // use 0 for "any"
//     size_t ai_addrlen;           // size of ai_addr in bytes
//     struct sockaddr *ai_addr;    // struct sockaddr_in ir in6
//     char *ai_canonname;          // full canonical hostname
//     struct addressinfo *ai_next; //linked list, next node
// };

// struct sockaddr
// {
//     unsigned short sa_family; // address family, AF_xxx
//     char sa_data[14];         // 14 bytes of protocol address
// };

// struct sockaddr_in
// {
//     short int sin_family;        // address family, AF_inet
//     unsigned short int sin_port; // port number
//     struct in_addr;              // internet address
//     unsigned char sin_zero[8];   // same size as struct sockaddr
// };

// int getaddrinfo(const char *node, const char *service, const struct addressinfo *hints, struct addressinfo **res);
// /*
//     three inputs:
//         Node: the host name to connect to:
//         Service: port name or other service
//         Hints: addressinfo struct that you have filled out relevant info
// */

// int socket(int domain, int type, int protocol);
// /*
//     three inputs:
//         what kind of socket? 1Pv4 or IPv6
//         stream or datagram
//         UDP or TCP?
// */

// int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
// /*
//     three inputs:
//         socketfd: the file descriptor you get from socket()
//         my_addr: pointer that directs to a struct sockaddr that consists the address, port # and other info
//         addrlen: the length of that address in bytes
// */

// int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
// /*
//     three inputs:
//         sockfd: socket file descriptor returned by socket()
//         serv_addr: the destination address
//         addrlen: the length of the destination address
// */

// int listen(int sockfd, int backlog);
// /*
//     listen to the socket file descriptor returned from socket()
//     backlog is the number of connections allowed in the queue
// */

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// /*
//     accept an incoming connection on a listening socket
//     sockfd: the listening() socket file descriptor
//     addr: the incoming connections address
//     addrlen: the length of the address
// */

// int send(int sockfd, const void *msg, int len, int flags);

// int recv(int sockfd, void *buf, int len, int flags);
// /*
//     sockfd: the socket that transmit/ receive messages
//     msg: messages to be transmitted
//     buf: the messages received
// */

// int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, socklen_t tolen);
// int recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
// /*
//     sockfd: the socket that transmit/receive messages
//     msg: messages to be transmitted
//     to: the destination address
//     from: the source adddress
// */

// char ip4[INET_ADDRSTRLEN]; // space to hold the IPV4 string
// struct sockaddr in sa;     // pretend this is loaded with someting
// inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

// int main()
// {
//     cout << "The Ipv4 address is: %s\n", ip4;

//     return 0;
// }