#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"

char* generate_random_string(int size)
{
    int i;
    const char set[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *res = malloc(size + 1);
    if(res)
    {
    for(i = 0; i < size; i++)
    {
        int key = rand() % (int) (sizeof set - 1);
        res[i] = set[key];
    }

    res[i] = '\0';
    }
    return res;
}



int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char message[BUFFER_SIZE];
    int ack_id;

    char **rand_word;
    int len_of_strings = 10;
    int i=0;
    int num_strings = 50;

     int len;

    /* Create local socket. */

    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }



    memset(&addr, 0, sizeof(addr));

    /* Connect socket to socket address. */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *) &addr,
                   sizeof(addr));
    if (ret == -1) {
        perror("The server is down");
        close(data_socket);
        exit(EXIT_FAILURE);
    }


    rand_word = (char **)malloc(num_strings*sizeof(char *));

    for(i=0; i<num_strings; i++)
    {
       rand_word[i] = generate_random_string(len_of_strings);
    }


     for(i=0;i<10;i++)
    {
      snprintf(message,71,"\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n",
                      i*5,
                        rand_word[i*5],
                          i*5+1,
                            rand_word[i*5+1],
                              i*5+2,
                               rand_word[i*5+2],
                                  i*5+3,
                                    rand_word[i*5+3],
                                       i*5+4,
                                          rand_word[i*5+4]);



        if(write(data_socket, message, strlen(message)) == -1)
         {
            perror("cannot write to socket");
            close(data_socket);
            exit(EXIT_FAILURE);  
         }
   
         if(read(data_socket, &ack_id, sizeof(int))==-1)
          {
                perror("read");
               close(data_socket);
                exit(EXIT_FAILURE);
          }

       printf("\n\nAcknowledged id %d received for strings whose ID is from  %d to %d from process P2\n",ack_id,i*5,i*5+4);


 
     }
    /* Close socket. */

    close(data_socket);

    exit(EXIT_SUCCESS);
}

