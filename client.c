#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define PORT 8000




main(int argc, char *argv[]){

/*CLIENT*/

   /*Definicoes*/
   struct sockaddr_in sock;
   struct hostent *hp;
   int port = PORT;
   int sckt;
   char buffer[100];

   /* Verificação + tutorial para uso */
   if(argc < 2)
   {
      printf("\n\tPara usar o programa: %s <IP_Addr>\n\n", argv[0]);
      exit(-1);
   }

   /* pegando IP passado por argumento */
   strncpy(buffer, argv[1], sizeof(buffer));
   hp = gethostbyname(buffer);

   /* Cria o socket */
   sckt = socket(AF_INET, SOCK_STREAM, 0);
   memset((char *) &sock, 0, sizeof(sock));
   memcpy(&sock.sin_addr, hp->h_addr, hp->h_length);
   sock.sin_family = hp->h_addrtype;
   sock.sin_port = htons(port);

   /* conecta client com o server */
   connect(sckt, (struct sockaddr *) &sock, sizeof(sock));

   /* Envia mensagem digitada pelo usuário */
   while(1)
   {
      printf("\nDigite sua mensagem: ");
      fflush(stdout);
      fgets(buffer, sizeof(buffer), stdin);

      send(sckt, buffer, strlen(buffer)+1, 0);

      if(strncmp(buffer, "SAIR", 4) == 0)   /*fecha conexao*/
      {
         close(sckt);
         break;
      }
   }
   close(sckt);
   return(0);
}
