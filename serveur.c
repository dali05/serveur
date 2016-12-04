#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write
#include <stdlib.h>
#include <pthread.h>
void *connection_handler(void *);
int main(int argc , char *argv[])
{
 int socket_desc , client_sock , c , read_size,nb=0;
 struct sockaddr_in server , client;
 int *new_sock;

 //Create socket
 socket_desc = socket(AF_INET , SOCK_STREAM , 0);
 if (socket_desc == -1)
 {
 printf("Could not create socket");
 }
 puts("Socket created");

 //Prepare the sockaddr_in structure
 server.sin_family = AF_INET;
 server.sin_addr.s_addr =INADDR_ANY;
 server.sin_port = htons(atoi(argv[1]));
 //Bind
 if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
 {
 //print the error message
 perror("bind failed. Error");
 return 1;
 }
 puts("bind done");

 //Listen
 listen(socket_desc , 3);

 //Accept and incoming connection
 puts("Waiting for incoming connections...");
 c = sizeof(struct sockaddr_in);

 //accept connection from an incoming client
 while(client_sock = accept(socket_desc, (struct sockaddr *)&client,
(socklen_t*)&c))
 {
 	 puts("Connection accepted");
 	 pthread_t sniff;
 	 new_sock=malloc(1);
 	 *new_sock=client_sock;
 	 nb++;
 	if(pthread_create(&sniff,NULL,connection_handler,(void *)new_sock)<0) 
 	{
 		perror("cloud not create thread");
 		return 1;
 	}
 puts("handler assigned");
 printf("client num %d est connecter\n",nb);
 }
 if (client_sock < 0)
 {
 perror("accept failed");
 return 1;
 }

 
 return 0;
}
void *connection_handler(void *socket_desc)
{
	int sock=*(int *)socket_desc;
	int read_size;
	 char client_message[2000];
	//printf("from %s %d SocketID=%d\n",inet_ntoa(client.sin_addr),client.sin_port,client_sock );
 //Receive a message from client
 while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
 {
 //Send the message back to client
 write(sock , client_message , strlen(client_message));
 }

 if(read_size == 0)
 {
 puts("Client disconnected");
 fflush(stdout);
 }
 else if(read_size == -1)
 {
 perror("recv failed");
 }

}
