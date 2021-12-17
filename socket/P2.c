#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un name;
    int ret;
    int connection_socket;
    int data_socket;
    char message[BUFFER_SIZE];
    int i;
    int ack_id;
    char **rand_word;
    int len_of_strings = 10;
    int num_strings = 50;
    int a[5];
    /* Create local socket. */
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&name, 0, sizeof(name));

    /* Bind socket to socket name. */

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *) &name,
               sizeof(name));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }



        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }


 rand_word = (char **)malloc(num_strings*sizeof(char *));

  for(i=0; i<num_strings; i++)
    {
       rand_word[i] = (char *)malloc(sizeof(len_of_strings+1));
    }


    for (i=0;i<10;i++)
 {


            ret = read(data_socket, message, sizeof(message));
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
   
             

            message[sizeof(message) - 1] = 0;


 
     
           printf("\n\n");
      printf("Message %d containing strings with their IDs in the format (ID:string) received from process P1 %s\n\n",i+1,message);


     sscanf(message,"\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n",
                      &a[0],
                        rand_word[i*5],
                          &a[1],
                            rand_word[i*5+1],
                              &a[2],
                               rand_word[i*5+2],
                                  &a[3],
                                    rand_word[i*5+3],
                                       &a[4],
                                          rand_word[i*5+4]);

     ack_id = a[4];
    if(write(data_socket, &ack_id, sizeof(int)) == -1)
       {
            perror("cannot write to socket");
            close(data_socket);
            exit(EXIT_FAILURE);
       }
    sleep(1);
  }




        close(data_socket);

        /* Quit on DOWN command. */


    close(connection_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}
