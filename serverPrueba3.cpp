/*
	C socket server example
*/

#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> //Para usar el inet_addr
#include<unistd.h>    //escribir
 
#include<pthread.h> //Para los hilos , hay que enlazar el lpthread

using namespace std;
/*
Para cargar servidor:

$ gcc serverResponde.c -o serverResponde -lpthread
$ ./serverResponde
*/

#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-variable"
char* directionFinder(char msg[]);


int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char* compareNoExiste = "No existe\n";
	char client_message[2000];
	
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8880 );
	
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
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
	
	    //Recibe mensajes del cliente
    while( (read_size = recv(client_sock , client_message, 2000 , 0)) > 0 )
    {	
		if(directionFinder(client_message) != compareNoExiste)
		{
			write(client_sock ,directionFinder(client_message) , strlen(directionFinder(client_message)));	
            puts("Encontrado");
            
	    }
	    else
	    {
            //Aquí debería crearse el socket a otro servidor
            puts("Redirigiendo...");
			write(client_sock , "No se encontró", strlen("No se encontró"));
		}    
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
	
	return 0;
}

char* directionFinder(char msg[])
{
	char* wordList1[]=		 {"Google.com\n","Youtube.com\n","Facebook.com\n"};
	char* wordAntonymList1[]={"123.46.023\n","127.454.923\n","234.123.12.1\n"};
	char* wordAntonym="No existe\n";
	for (int i = 0;i<3;i++)
	{
		if(*msg==*wordList1[i])
			wordAntonym=wordAntonymList1[i];
		if(*msg==*wordAntonymList1[i])
			wordAntonym=wordList1[i];
	}
	return wordAntonym;
}

