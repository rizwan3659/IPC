#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define PIPE_NAME "/tmp/pipe1"
#define PIPE_NAME2 "/tmp/pipe2"
#define BUFFER_SIZE 256



int main(int argc, char *argv[])
{
    int ret;
    char message[BUFFER_SIZE];
    int i;
    int fd,fd2;
    int ack_id;
    char **rand_word;
    int len_of_strings = 10;
    int num_strings = 50;
    int a[5];

      umask(0);
        if(mknod(PIPE_NAME, S_IFIFO | 0660, 0) == -1){
                if(errno != EEXIST){
                        perror("Cannot create the pipe");
                        exit(EXIT_FAILURE);
                }
                else
                        printf("Using existing file\n");
        }


  if(mknod(PIPE_NAME2, S_IFIFO | 0660, 0) == -1){
                if(errno != EEXIST){
                        perror("Cannot create the pipe");
                        exit(EXIT_FAILURE);
                }
                else
                        printf("Using existing file\n");
        }




 rand_word = (char **)malloc(num_strings*sizeof(char *));

  for(i=0; i<num_strings; i++)
    {
       rand_word[i] = (char *)malloc(sizeof(len_of_strings+1));
    }


     fd = open(PIPE_NAME, O_RDONLY);
    if(fd == -1)
   {
    perror("cannot open pipe");
    exit(EXIT_FAILURE);
   }


   fd2 = open(PIPE_NAME2, O_WRONLY);
     if(fd2==-1)
    {

     perror("cannot open pipe 2");
     exit(EXIT_FAILURE);
   }


    for (i=0;i<10;i++)
 {

// reading message from FIFO /tmp/pipe1
            ret = read(fd, message, sizeof(message));
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
   
             

            message[sizeof(message) - 1] = 0;


 
     
           printf("\n\n");
      printf("Message %d containing strings with their IDs in the format (ID:string) received from process P1 %s\n\n",i+1,message);

//scanning ID and strings from message and storing into arrays of ID and strings
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
// writing acknowledge id (highest id of strings recieved) message to FIFO /tmp/pipe2
    if(write(fd2, &ack_id, sizeof(int)) == -1)
       {
            perror("cannot write to socket");
            close(fd);
            exit(EXIT_FAILURE);
       }
    sleep(1);
  }



  close(fd);
  close(fd2);
    exit(EXIT_SUCCESS);
}
