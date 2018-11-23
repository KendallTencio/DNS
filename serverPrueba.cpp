/*
	C socket server example
*/

#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> //Para usar el inet_addr
#include<unistd.h>    //escribir
#include"SplayTree.cpp"
 
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
char* irASiguienteServer(char message[]);
char* splayTreeFinder(char msg[]);

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char* compareNoExiste = "No existe\n";
	char client_message[2000];
	
	
	//Crear socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("No se pudo crear el socket");
	}
	puts("Socket creado");
	
	//Preparar la estructura del sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 9991 );
	
	//Hacer el Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind fallo. Error");
		return 1;
	}
	puts("bind hecho");
	
	//"Listen" a un socket
	listen(socket_desc , 3);
	
	while(1)
	{	
		//Aceptar conexiones entrantes
		puts("Esperando por conexiones entrantes...");
		c = sizeof(struct sockaddr_in);
	
		//Aceptar la conexión de un cliente 
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
		perror("Conexion fallida");
		return 1;
		}
		puts("Conexion aceptada");

		//Recibe mensajes del cliente
		while( (read_size = recv(client_sock , client_message, 2000 , 0)) > 0 )
		{	
			if(directionFinder(client_message) != compareNoExiste)
			{
				write(client_sock ,splayTreeFinder(client_message) , strlen(splayTreeFinder(client_message)));	
				puts("IP encontrado");
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
			perror("recv fallida");
		}
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
		if(*msg == *wordList1[i])
			wordAntonym=wordAntonymList1[i];
		if(*msg == *wordAntonymList1[i])
			wordAntonym=wordList1[i];
	}
	return wordAntonym;
}

char* splayTreeFinder(char msg[])
{
	char const* nombreRecibido = msg;
	char* wordAntonym="No existe\n";
	struct Nodo *raiz=NULL;
	//char llave[] = "facebook.com";
	raiz=insertar(raiz, 1, "youtube.com", "1");
	raiz=insertar(raiz, 2, "facebook.com", "2");
	raiz=insertar(raiz, 3, "google.com","3");
	raiz=insertar(raiz, 4, "myspace.com","4");
	raiz=insertar(raiz, 5, "instagram.com","5");
	raiz=buscar(raiz,1);
	cout<<raiz->nombre;
	cout<<nombreRecibido;
	if (raiz->nombre[1]==nombreRecibido[1]){
		printf("¡Encontrado!");
		wordAntonym = raiz->llaveChar;
	}
	else{
		printf("%s\n",raiz->nombre);
		printf("Esa llave no existe\n");
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
	server.sin_port = htons( 8889 );

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
