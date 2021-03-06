/*
	C socket server example
*/

#include<stdio.h>  
#include<string.h>
#include<stdlib.h>    
#include<sys/socket.h>	  
#include<arpa/inet.h>   //Para usar el inet_addr
#include<unistd.h>      //escribir						   	
#include<pthread.h>     //Para los hilos , hay que enlazar el lpthread
#include"SplayTree.cpp"

using namespace std;
/*
Para cargar servidor:
$ gcc serverResponde.c -o serverResponde -lpthread
$ ./serverResponde
*/

#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-variable"
char* directionFinder(char msg[]);
char* irASiguienteServer(char message[]);
int irASiguienteServer();

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
	server.sin_port = htons( 8889 );
	
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
	
	while(1)
	{
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
				//write(client_sock , "Me voy a otro servidor", strlen("Me voy a otro servidor"));
				char* ipEncontradaEnOtro = irASiguienteServer(client_message);
				puts("Redireccionando de regreso...");
				write(client_sock , ipEncontradaEnOtro, strlen(ipEncontradaEnOtro));
			}    
		}
	
		if(read_size == 0)
		{
			puts("Cliente desconectado");
			fflush(stdout);
		}
		else if(read_size == -1)
		{
			perror("recv fallo");
		}
	}
	return 0;
}

char* directionFinder(char msg[])
{
	char* wordList1[]=		 {"Estructuras.com\n","Repretel.com\n","Instagram.com\n"};
	char* wordAntonymList1[]={"111.11.111\n","666.666.666\n","777.777.77\n"};
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

char* splayTreeFinder(char msg[])
{
	const char* nombreRecibido = msg;
	const char* nombre1 = "myspace.com";
	const char* nombre2 = "instagram.com";
	char* wordAntonym="No existe\n";
	struct Nodo *raiz=NULL;
	//char llave[] = "facebook.com";
	raiz=insertar(raiz, 4, nombre1,"4");
	raiz=insertar(raiz, 5, nombre2,"5");
	raiz=buscar(raiz,1);

	if (strcmp(raiz->nombre,nombreRecibido) == 0){
		cout<<"¡Encontrado!"<<endl;
		wordAntonym = raiz->llaveChar;
	}
	else{
		cout<<raiz->nombre<<endl;
		cout<<"Esa llave no existe\n"<<endl;
		wordAntonym="No existe\n";
	}
	return wordAntonym;
}

char* irASiguienteServer(char message[]){
	int sock;
	struct sockaddr_in server;
	char server_reply[2000];
	char* replyReturn;
	
	//Crear el socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("No se pudo crear socket");
	}
	puts("Socket creado");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8880 );

	//Conextar a servidor remoto
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Fallo la conexion. Error");
		return "";
	}
	
	puts("Conectado\n");
	int x = 1;
	//Mantiene la comunicación con el servidor
	while(x == 1)
	{
		//Envía el mensaje
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Fallo envio");
			return "";
		}
		
		//Recibe una respuesta del servidor
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("Fallo recv");
			break;
		}
		
		//puts("Server reply :");
		puts(server_reply);
		x = 0;
		replyReturn = server_reply;
	}
	
	close(sock);	
	return replyReturn;
}
