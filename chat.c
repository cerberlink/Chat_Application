#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

int menu();
void help();
void myip(struct addrinfo *);
void myport(char *);

struct structure
{
    int sockfd; // socket file descriptor
    char *serverPort;
    struct addrinfo *servinfo;
    struct sockaddr_storage client_addr;
};

//void *server_function(void *)
//{

//}

void *client_function(void *argument)
{
    struct structure *clientarg = argument;

    switch (menu())
    {
        case 1: help();
                break;
        case 2: myip(clientarg->servinfo);
                break;
        case 3: myport(clientarg->serverPort);
                break;
        case 4: // call connect function
                break;
        case 5: // call list function
                break;
        case 6: // call terminate function
                break;
        case 7: // call send function
                break;
        case 8: // call exit function
                break;
        default:
                break;
    }
}



int main(int argc, char *argv[])
{    
    int status = 0;
    int enable = 1;

    struct addrinfo hints;

    struct structure *argument;
    argument->serverPort = argc > 1 ? argv[1] : "0"; // if port number is not specified, use next available port number

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6 not specified
    hints.ai_flags = AI_PASSIVE;     // returned socket addresses will be suitable for binding
    hints.ai_socktype = SOCK_STREAM; // TCP
    
    if ((status = getaddrinfo(NULL, argument->serverPort, &hints, &argument->servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    for (struct addrinfo *i = argument->servinfo; i; i = i->ai_next) // find first available address
    {
	argument->sockfd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);

	if (argument->sockfd == -1)
	{
	    perror("server: socket");
	    continue;
	}

	if (setsockopt(argument->sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
	{
	    perror("server: setsockopt");
	    return 1;
	}

	if (bind(argument->sockfd, i->ai_addr, i->ai_addrlen) == -1)
	{
	    close(argument->sockfd);
	    perror("server: bind");
	    continue;
	}

	break;
    }

    pthread_t tid, servtid, clienttid;

    //if (pthread_create(&servtid, NULL, server_function, NULL))
    //{
	//perror("error: create server");
    //}

    if (pthread_create(&clienttid, NULL, &client_function, argument))
    {
	perror("error: create client");
    }

    tid = clienttid;

    

    if (listen(argument->sockfd, 10) == -1)
    {
	perror("listen");
	exit(1);
    }    

    if (pthread_join(servtid, NULL) == 0 && pthread_join(clienttid, NULL) == 0)
    {
	pthread_exit(NULL);
    }

    freeaddrinfo(argument->servinfo);

    return 0;
}

int menu()
{
    int choice;
    
    printf("Chat Application for Remote Message Exchange\n\n");

    printf("1) Help\n");
    printf("2) My IP\n");
    printf("3) My Port\n");
    printf("4) Connect\n");
    printf("5) List\n");
    printf("6) Terminate\n");
    printf("7) Send\n");
    printf("8) Exit\n\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    return choice;
}

void help()
{
    printf("myip: display IP address\n");
    printf("myport: display port number\n");
    printf("connect: connect to another peer\n");
    printf("list: display all connections\n");
    printf("terminate: terminate a connection\n");
    printf("send: send messages to peers\n");
    printf("exit: exit the program\n\n");
}

void myip(struct addrinfo *servinfo)
{    
    char buffer[INET6_ADDRSTRLEN];

    inet_ntop(servinfo->ai_family, &servinfo, buffer, INET6_ADDRSTRLEN);

    printf("%s\n\n", buffer);
}

void myport(char *port)
{
    printf("%s\n\n", port);
}

/* int connect (serverName, char[] serverPort)
{

}

list

terminate <connection id>

send <connection id> <message>
{
    size_t sz = 0;
    char *str = NULL;

    printf("Enter the string : ");
    getline(&str, &sz, stdin);

    write(sockfd, str, sz);

    printf("%s", str);

    free(str);
}

exit */