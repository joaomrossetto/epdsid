#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

#define PORT 8000



void printMensagem(char mensagem[100]){
   printf("\nMensagem enviada pelo cliente: \n%s\n", mensagem);
   printf("\nA thread tem seu numero como %d",pthread_self()); //endereço de thread da biblioteca pthread
}

void mandaThread(int alternador, char mensagem[100], pthread_t threads[2]){
   char texto[100]; 
   strncpy(texto, mensagem, 18); // recebe a mensagem enviada pelo cliente e a copia em uma string local
   pthread_create(&threads[alternador%2], NULL, printMensagem, texto); //cria a thread para o valor do alternador, printando a mensagem
}

main(void){

/* SERVER*/

   /*definições*/  
   struct sockaddr_in sock;
   int len=sizeof(sock);
   struct hostent *hp;
   int port = PORT;
   int acc, soc;
   char buffer[100];

   /*pthread*/

   pthread_t threads[2];
   int alt = 0; //alternador

   /* Cria o socket*/
   if((acc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   memset((char *) &sock, 0, sizeof(sock));
   sock.sin_addr.s_addr = htonl(INADDR_ANY);
   sock.sin_port = htons(port);
   sock.sin_family = AF_INET;
   bind(acc, (struct sockaddr *) &sock, len);

   /* aguarda conexao */
   listen(acc, 5);
   soc = accept(acc, (struct sockaddr *) &sock, &len);
   close(acc);
   printf("Conexão: %d", soc);


   /* ouve mensagens do cliente até "SAIR"*/
   while(1)
   {
      recv(soc, buffer, sizeof(buffer), 0);

      mandaThread(alt,buffer, threads);
      alt = alt + 1;   

      if(strncmp(buffer, "SAIR", 4) == 0)   /* fecha o scoket */
      {
         puts("Fechando Conexão com o client!");
         close(soc);
         break;
      }
   }
   return(0);
}
