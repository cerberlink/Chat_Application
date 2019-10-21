#include<stdio.h>
#include <stdlib.h>
//for socket and related functions
#include <sys/types.h>
#include <sys/socket.h>
//for inncluding structures which will store information needed
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

struct sockaddr_in sa;                                          // IPv4
struct sockaddr_in6 sa6;                                        // IPv6
// inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));             //IPv4
// inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); //IPv6

#define PORT "3490" // the port client will be connecting
#define MAXDATASIZE 100 // max nnumber of bytes we can get 

int main()
{
    // creating the socket
    int network_socket;
    //calling socket function and storing the result in the variable
    //socket(domainOfTheSocket,TypeOfTheSocket,FlagForProtocol{0 for default protocol i.e, TCP})
    //AF_INET = constant defined in the header file for us
    //TCP vs UDP --- SOCK_STREAM for TCP
    // flag 0 for TCP (default protocol)
    network_socket = socket(AF_INET,SOCK_STREAM,0);
    //creating network connection
    //in order to connect to the other side of the socket we need to declare connect
    //with specifying address where we want to connect to
    //already defined struct sockaddr_in
    struct sockaddr_in server_address;
    //what type of address we are woking with
    server_address.sin_family = AF_INET;
    //for taking the port number and htons converts the port # to the appropriate data type we want to write
    //to specifying the port
    //htons : conversion functions

    server_address.sin_port = htons(9002);
    //structure within structure A.B.c
    server_address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY is for connection with 0000
    //connect returns us a response that connection is establlised or not
    int connect_status = connect(network_socket,(struct sockaddr *) &server_address, sizeof(server_address));
    //check for the error with the connection

    if (connect_status == -1){
    printf("There was an error making a connection to socket\n" );
    }

    //recieve data from the server
    char server_response[256];

    //recieve the data from the server
    recv(network_socket,&server_response,sizeof(server_response),0);

    //recieved data from the server successfully then printing the data obtained from the server

    printf("Ther server sent the data : %s",server_response);

    //closing the socket
    close(network_socket);
    return 0;
    // int sockfd;
    // struct addrinfo hints, *servinfo, *p;
    // int rv;
    // int numbytes;
    // struct sockaddr_storage their_addr;
    // char buf[MAXBUFLEN];
    // socklen_t addr_len;
    // char s[INET6_ADDRSTRLEN];
    // memset(&hints, 0, sizeof hints);
    // hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    // hints.ai_socktype = SOCK_DGRAM;
    // hints.ai_flags = AI_PASSIVE; // use my IP
    // if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
    // fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    // return 1;
    // }
    // // loop through all the results and bind to the first we can
    // for(p = servinfo; p != NULL; p = p->ai_next) {
    //     if ((sockfd = socket(p->ai_family, p->ai_socktype,
    //         p->ai_protocol)) == -1) {
    //     perror("listener: socket");
    //     continue;
    //     }
    //     if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    //         close(sockfd);
    //         perror("listener: bind");
    //         continue;
    //     }
    //     break;
    // }
    // if (p == NULL) {
    //     fprintf(stderr, "listener: failed to bind socket\n");
    //     return 2;
    // }
    // freeaddrinfo(servinfo);
    // printf("listener: waiting to recvfrom...\n");

    // addr_len = sizeof their_addr;
    // if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {perror("recvfrom");
    // exit(1);
    // }
    // printf("listener: got packet from %s\n",
    //     inet_ntop(their_addr.ss_family,
    //         get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
    // printf("listener: packet is %d bytes long\n", numbytes);
    // buf[numbytes] = '\0';
    // printf("listener: packet contains \"%s\"\n", buf);
    // close(sockfd);
    // return 0;
}

// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>
// get sockaddr, IPv4 or IPv6
// void *get_in_addr(struct sockaddr *sa)
// {
//     if(sa ->sa_family == AF_INET)
//     {
//         return &(((struct sockaddr_in*)sa)->sin_addr);
//     }
//     return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }
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

// // color info extension VSC