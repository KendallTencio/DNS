//SERVIDOR DNS 1
//Código extraído, modificado y basado en el de: https://www.binarytides.com/server-client-example-c-sockets-linux/
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
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *mensaje;
     
    //Crea un socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("No se pudo crear el socket");
    }
     
    //Preparar la estructura sockaddr_in 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 9991 );
     
    //Realiza el Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind ha fallado");
        return 1;
    }
    puts("bind realizado");
     
    //Realiza el Listen
    listen(socket_desc , 3);
     
    //Acepta una conexión que llega
    puts("Esperando conexiones...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Conexión aceptada");
         
        //Respuesta a un cliente
        mensaje = "Hola cliente, he recibido su conexion, ahora asignare un handler\n";
        write(new_socket , mensaje , strlen(mensaje));
         
        pthread_t sniffer_thread;
        new_sock = (int*) malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("No se pudo crear hilo");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler asignado");
    }
     
    if (new_socket<0)
    {
        perror("Ha fallado entrada");
        return 1;
    }
     
    return 0;
}
 
 //En esta zona se maneja la conexión de cada cliente

void *connection_handler(void *socket_desc)
{
    //Obtiene el descriptor del socket
    int sock = *(int*)socket_desc;
    int read_size;
    char *mensaje , client_mensaje[2000];
    char *mensajeDeCliente = "Hola";
     
    //Envía mensaje al cliente
    mensaje = "¡Hola!, soy su handler de conexión\n";
    write(sock , mensaje , strlen(mensaje));
     
    mensaje = "Escriba una palabra y buscare su antonimo\n";
    write(sock , mensaje , strlen(mensaje));
     
    //Recibe mensajes del cliente
    while( (read_size = recv(sock , client_mensaje , 2000 , 0)) > 0 )
    {	
				
				write(sock ,directionFinder(client_mensaje) , strlen(directionFinder(client_mensaje)));	
			     
    }
     
    if(read_size == 0)
    {
        puts("Cliente desconectado");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv falló");
    }
         
    //Libera el puntero del socket
    free(socket_desc);
     
    return 0;
}


char* directionFinder(char msg[])
{
	char* wordList1[]=		 {"Google.com\n","Youtube.com\n","Facebook.com\n"};
	char* wordAntonymList1[]={"123.46.023\n","127.454.923\n","234.123.12.1\n"};
	char* wordAntonym="No_Existe\n";
	for (int i = 0;i<3;i++)
	{
		if(*msg==*wordList1[i])
			wordAntonym=wordAntonymList1[i];
		if(*msg==*wordAntonymList1[i])
			wordAntonym=wordList1[i];
	}
	return wordAntonym;
}
