//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux  
//written by Daniel Cardenas

#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
#include <sys/select.h>
#include <unistd.h>

//testing to get IP
#include <netdb.h>
#include <arpa/inet.h>

#define STDIN  0    
#define TRUE   1  
#define FALSE  0  
#define PORT 8888  
     
int main(int argc, char *argv[])   
{   
    int opt = TRUE;   
    int addrlen , new_socket , connection_socket[30] , max_clients = 30 , activity, i , valread , sd;   
    int max_sd;   
	int server_port = atoi(argv[1]);
	int socket_to_test = 0;
	int socket_receiving = 0;
	//printf("\n User Port Number: %d\n", server_port);
	
	//initialise all connection_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        connection_socket[i] = 0;   
    }   
    
    char buffer[256];  //data buffer of 1K  
         
    fd_set readfds;     
        
    //create a master socket  
	int master_socket;
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
	
    
	/* ******* GETTING IP ADDRESS ********* */
	char hostbuffer[256]; 
	char *IPbuffer; 
	struct hostent *host_entry; 
	int hostname; 
  
	// To retrieve hostname 
	hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
  
	// To retrieve host information 
	host_entry = gethostbyname(hostbuffer); 
  
	// To convert an Internet network 
	// address into ASCII string 
	IPbuffer = inet_ntoa(*((struct in_addr*) 
						   host_entry->h_addr_list[0]));
	/*****************************************/
	
	
	/*
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }  */
	
     
    //type of socket created  
	struct sockaddr_in address;
	
	bzero(&address, sizeof(address));
	
    address.sin_family = AF_INET;   //IPv4
    address.sin_addr.s_addr = INADDR_ANY;  //any address on this computer  
    address.sin_port = htons(server_port);    // port is provided by user
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", server_port);   
	
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
    
	
	//accept the incoming connection  
	//size of address is used to accept new socket
	addrlen = sizeof(address);   
	puts("Waiting for connections ..."); 
	
	  
    while(TRUE)   
    {   
        //clear the socket set  
        FD_ZERO(&readfds);   
		
		//TESTING TO ADD STDIN
		FD_SET(STDIN, &readfds);
		
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
		
        max_sd = master_socket;
		
		/***** TESTING A SINGLE CLIENT *****/
		/*
		//add connection socket from incoming client
		if (socket_to_test > 0) {
			FD_SET( socket_to_test , &readfds);
			//printf("The SOCKET TO TEST WAS SET INTO readfds SUCCESSFULLY!");
		}
		if(socket_to_test > max_sd)
			max_sd = socket_to_test;
		*/
		/************************************* */
		
		
		//add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = connection_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        } 
		
		
		//wait for an activity on one o fhte sockets, 
		activity = select(max_sd+1 , &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}

		/* **** CHECKING COMMAND LINE COMMANDS ***** */
		
		if (FD_ISSET(STDIN, &readfds))
		{
			//printf("\nThere is something to read!\n");
			int valread = read(STDIN_FILENO, buffer, sizeof(buffer));  //CHANGED FROM '1024' TO sizeof(buffer)  TODO TODO
			if (valread == 0)
			{
				printf("valread is 0, thus number of bytes should be zero\n");
			}
			else
			{
				int index;
				int count = 0;
				
				/*
				char** stringArray;
				stringArray = malloc(sizeof(char*) * 3);
				for (index = 0; index < 3; index++) {
					stringArray[index] = malloc(20*sizeof(char));
				} */
				
				
				buffer[valread] = '\0';
				//printf("Your input was: %s\n", buffer);
				
				char* token = strtok(buffer, " ");
				while(token != NULL)  //TODO TODO TODO GET RID OF THIS WHILE LOOP, I THINK ITS UNNECESSARY!!!
				{   
					//printf("%s\n", token);
					
					if ((strcmp(token, "help\n")) == 0) {
						//something for help
						printf("This is your help, You're Welcome!\n");
						
						break;
					}
					else if ((strcmp(token, "myip\n")) == 0) {
						//something for myip
						printf("My IP is: %s\n", IPbuffer);
						break;
					}
					else if ((strcmp(token, "myport\n")) == 0) {
						//something for myport
						//printf("myport input\n");
						printf("\nMy Port Number is: %d\n", server_port);
						break;
					}
					else if ((strcmp(token, "connect")) == 0) {  //TODO TODO TODO
						//something for connect
						//printf("Entered connect method!!\n");
						
						char* ip_address_connect;
						char* temp_port;
						int port_number_connect;
						ip_address_connect = strtok(NULL, " ");
						printf("\nYour connect to IP address is: %s", ip_address_connect);
						temp_port = strtok(NULL, " ");
						printf("\nYour connect to Port Number is: %s", temp_port);
						port_number_connect = atoi(temp_port);
						
						//create a socket
						int network_socket;
						network_socket = socket(AF_INET, SOCK_STREAM, 0);
						
						//specify an address for the socket
						struct sockaddr_in server_address;
						server_address.sin_family = AF_INET;
						server_address.sin_port = htons(port_number_connect);
						server_address.sin_addr.s_addr = inet_addr(ip_address_connect);
						
						int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
						if (connection_status == -1) {
							printf("There was an error making a connection to the remot socket \n\n");
						}
						else {
							printf("Connection was successfull\n\n");
						}
						
						/**** TESTING A SINGLE CLIENT *****/
						//socket_to_test = network_socket;
						
						
						
						//add new socket to array of sockets  
						for (i = 0; i < max_clients; i++)   
						{   
							//if position is empty  
							if( connection_socket[i] == 0 )   
							{   
								connection_socket[i] = network_socket;   
								printf("Adding to list of sockets as %d\n" , i);   
									 
								break;   
							}   
						}
						
						//printf("Closing socket %d\n", network_socket);
						//close(network_socket);
						
						break;
					}
					else if ((strcmp(token, "list\n")) == 0) {
						//something for list
						printf("HERE IS THE LIST OF CONNECTION SOCKETS!!\n\n");
						
						//bzero(&address, sizeof(address));
						
						// cycle through connection list
						for (i = 0; i < max_clients; i++)   
						{   
							//if position is empty  
							if( connection_socket[i] > 0 )   
							{  
								sd = connection_socket[i];
								getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
								printf("%d: %s \t %d\n", i, inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
							}   
						}
						
						
						break;
					}
					else if ((strcmp(token, "terminate\n")) == 0) {
						//something for terminate
						printf("terminate input\n");
						/****** TEMPORARY TESTING *****/
						printf("The socket to test has fd of:  %d\n\n", socket_to_test);
						break;
					}
					else if ((strcmp(token, "send")) == 0) {
						//something for send
						//printf("send input\n");
						
						/******TESTING TESTING TESTING ****/
						char* id_temp = strtok(NULL, " ");
						int id = atoi(id_temp);  // id USED FOR CONNECTION LIST (INDEX OF CONNECTION_SOCKET ARRAY)
						char* input = strtok(NULL, " ");
						char messageArray[256] = "";  // messageArray USED FOR MESSAGE WE ARE SENDING
						int counter = 0;
						while(input != NULL) {
							strcat(messageArray, input);
							strcat(messageArray, " ");
							input = strtok(NULL, " ");
						}
						//printf("\nMy Message Is: %s\n", messageArray);
						//printf("\nMy ID is: %d\n", id);
						int socket_test = connection_socket[id];
						//send(sd , buffer , strlen(buffer) , 0 );
						//int message_length = strlen(messageArray);
						//printf("\nMessage length is: %d\n", message_length);
						send(socket_test , messageArray, strlen(messageArray) , 0);  // multiple connection send
						//clearing messageArray
						memset(messageArray, 0, sizeof(messageArray));
						printf("Message sent!\n");
						
						/**** TESTING A SINGLE CLIENT *****/
						//send(socket_to_test , messageArray, strlen(messageArray) , 0);
						
						/**********************************/
						
						break;
					}
					else if ((strcmp(token, "exit\n")) == 0) {
						printf("EXITING THE APPLICATION\n");
						
						//closing all connection sockets
						// cycle through connection list
						for (i = 0; i < max_clients; i++)   
						{   
							//if position is empty  
							if( connection_socket[i] > 0 )   
							{  
								sd = connection_socket[i];
								close(sd);
							}   
						}
						
						exit(0);
					}
					else {
						//incorrect input
						printf("incorrect input\n");
						break;
					}
					
					//token = strtok(NULL, " ");
				} //WHILE MENU LOOP
				
				
				
			}
			
		}
		
		//FOR THE LISTENING SOCKET
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
            
			/**** TESTING A SINGLE CLIENT *****/
			//socket_to_test = new_socket;
			
			
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( connection_socket[i] == 0 )   
                {   
                    connection_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            } 
			
			
			
		}			
		
		/**** TESTING A MULTIPLE CLIENTS *****/
		
		for (i = 0; i < max_clients; i++)   
        {
			sd = connection_socket[i];
			if (FD_ISSET( sd , &readfds) && (sd > 0))
			{
				char buffer_recv[256];
				//receive message in 'buffer_recv' and 'rval' holds status
				int rval = recv(sd , buffer_recv , sizeof(buffer_recv) , 0 ); 				
				if (rval > 0)  //a message is being sent
				{
					int b_length = strlen(buffer_recv);
					printf("Length of the buffer_recv: %d\n", b_length);
					/*
					for (i=0; i < b_length; i++)
					{
						printf("char %d: %s\n", i, buffer_recv[i]);
					} */
					printf("Message Received: %s\n", buffer_recv);
					memset(buffer_recv, 0, sizeof(buffer_recv));
					printf("End of Message Received.\n");
				}
				else if (rval == 0)  //other client is disconnecting
				{
					//Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&address , 
                        (socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    connection_socket[i] = 0;
					
				}
				else if (rval < 0)  //error in receiving
				{
					//error message
					printf("There was an ERROR in receiving!\n");
					//int valread = read( sd , buffer, 1024);
					//printf("This was valread:  %d\n", valread);
				}
				//printf("\nMessage from other person: %s\n\n", buffer);
			}
		}
		
		
		
		/*
		 //else its some IO operation on some other socket 
        for (i = 0; i < max_clients; i++)   
        {   
            sd = connection_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {   
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&address , 
                        (socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    connection_socket[i] = 0;   
                }   
                     
                //display the message  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    //buffer[valread] = '\0';   
					//char message_response[100];
                    //recv(sd , message_response , sizeof(message_response) , 0 );  
					//printf("\nMessage from other person: %s\n\n", message_response);
					printf("Someone is trying to send you a message\n");
					buffer[valread] = '\0';
					//recv(sd , buffer , strlen(buffer) , 0 );  
					printf("\nMessage from other person: %s\n\n", buffer);
					
                }   
            }   
        }   
		*/
	
	
		//printf("REACHED END OF WHILE LOOP\n");
    }  // infinite while loop   
         
    return 0;   
}   