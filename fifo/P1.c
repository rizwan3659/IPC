#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define PIPE_NAME "/tmp/pipe1"
#define PIPE_NAME2 "/tmp/pipe2"
#define BUFFER_SIZE 256


// This function generates random string of fixed length
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
    int ret;
    char message[BUFFER_SIZE];
    int ack_id;
    int fd,fd2;
    char **rand_word;
    int len_of_strings = 10;
    int i=0;
    int num_strings = 50;

     int len;

    // Creating the pipe
    // umask(0) will clean the inherited permission. now any permission can be set(read, write, execute) 

        umask(0);
//mknod() call that creates a FIFO(named pipe)
//Second argument is the creation mode, which is used to tell mknod() to make a FIFO (the S_IFIFO part of the OR) and sets access permissions to that file (octal 644, or rw-r--r--) 
        if(mknod(PIPE_NAME, S_IFIFO | 0660, 0) == -1){
                if(errno != EEXIST){
                        perror("Cannot create the pipe");
                        exit(EXIT_FAILURE);
                }
                else
                        printf("Using existing file\n");
        }
// we are used 2 named pipes for bidirectional transfer of data
      if(mknod(PIPE_NAME2, S_IFIFO | 0660, 0) == -1){
                if(errno != EEXIST){
                        perror("Cannot create the pipe");
                        exit(EXIT_FAILURE);
                }
                else
                        printf("Using existing file\n");
        }




//allocates 50 pointers
    rand_word = (char **)malloc(num_strings*sizeof(char *));

//created array of 50 random strings (of characters) of fixed length 10.
    for(i=0; i<num_strings; i++)
    {
       rand_word[i] = generate_random_string(len_of_strings);
    }

//used  open  system call for  writing into the FIFO file /tmp/pipe1. Cannot read from the FIFO file.	.
      fd = open(PIPE_NAME, O_WRONLY);
     if(fd==-1)
    {

     perror("cannot open pipe 1");
     exit(EXIT_FAILURE);
   }

//used  open  system call for  reading into the FIFO file /tmp/pipe2. Cannot write into the file.   
      fd2 = open(PIPE_NAME2, O_RDONLY);
     if(fd2==-1)
    {

     perror("cannot open pipe 2");
     exit(EXIT_FAILURE);
   } 
     for(i=0;i<10;i++)
    {
    // construct the message using 5 random strings along with their ID.
      sprintf(message,"\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n",
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

   // writing this message to FIFO /tmp/pipe1
        if(write(fd, message, strlen(message)) == -1)
         {
            perror("cannot write to pipe");
            close(fd);
            exit(EXIT_FAILURE);  
         }
// reading aknowledge ID from the FIFO /tmp/pipe2
         if(read(fd2, &ack_id, sizeof(int))==-1)
          {
                perror("read");
               close(fd2);
                exit(EXIT_FAILURE);
          }

       printf("\n\nAcknowledged id %d received for strings whose ID is from  %d to %d from process P2\n",ack_id,i*5,i*5+4);

    sleep(1); 
     }
    /* Close socket. */

   close(fd);
   close(fd2);

    exit(EXIT_SUCCESS);
}

