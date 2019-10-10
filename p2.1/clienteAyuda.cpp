#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <cstdio>
#include "macros.hpp"
// void partida(int const & sd);
int main(int argc, char **argv)
{
	if(argc<2)
	{
		std::cout<<BIRED<<"ERROR se debe introducir la IP como argumento"<<RESET<<std::endl;
		return -1;
	}

   int sd;
   struct sockaddr_in sockname;
   char buffer[250];
   std::memset (buffer,'\0',250);
   char buffer1[250];
   std::memset (buffer1,'\0',250);
   socklen_t len_sockname;

   sd = socket (AF_INET, SOCK_STREAM, 0);
   if (sd == -1)
   {
      std::cout << BIRED << "No se puede abrir el socket cliente" << RESET << std::endl;
      exit (1);
   }

   sockname.sin_family = AF_INET;
   sockname.sin_port = htons(2050);
   sockname.sin_addr.s_addr =  inet_addr(argv[1]);

   len_sockname = sizeof(sockname);

	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		std::cout << BIRED << "Error de conexión" << RESET << std::endl;
		exit(1);
	}

   int opcion;
   std::string cadena, usuario, passwd, registro;
   recv(sd, buffer, 250, 0);

   if(strcmp("Demasiados clientes conectados\0", buffer)==0)
   {
      std::cout << BIRED << buffer << RESET << std::endl;
      return -1;
   }
   else std::cout << BIYELLOW << buffer << RESET << "\n";//mensaje conexión correcta


   bzero(buffer,sizeof(buffer));
   bzero(buffer1, sizeof(buffer1));

   while(1)
   {
      bzero(buffer,sizeof(buffer));
      bzero(buffer1, sizeof(buffer1));
      fgets(buffer,sizeof(buffer),stdin);

      send(sd, buffer, sizeof(buffer), 0);
      recv(sd, buffer1, sizeof(buffer1), 0);

      if(strcmp(buffer1, "+OK Waiting for a player\0")==0)
      {
         std::cout << BIBLUE << buffer1 << RESET << '\n';
         bzero(buffer1, sizeof(buffer1));
         recv(sd, buffer1, sizeof(buffer1), 0);
      }

      if(strcmp(buffer1, "SALIR\0")==0)
         break;

      //else std::cout << BIRED << buffer1 << RESET << '\n';
      else
      {
      	if(strstr(buffer1,"-Err")!=NULL)
      		std::cout << BIRED << buffer1 << RESET << std::endl;
      	else
      	{
      		if(strstr(buffer1,"+OK")!=NULL)
      			std::cout << BIGREEN << buffer1 << RESET << std::endl;
      		else std::cout << buffer1 << std::endl;
      	}
      }
   }

}
