#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include "message.h"
#include<unistd.h>

#define KEY1 0x01
#define KEY2 0x02


// This Funtion creates message queue and returns  message queue ID or -1 on failure.
int get_message_queue(char id)
{
//key is a system-wide unique identifier describing the queue. Every other process that who wants to  connect to this queue will have to use the same key.
        key_t queue_key = ftok(".", id);
        printf("Created key %X\n", queue_key);

        // Get message queue
        int queue_id;
//2nd Argument is set equal to IPC_CREAT bit-wise OR'd with the permissions(0660) for this queue
        if((queue_id = msgget(queue_key, IPC_CREAT | 0660)) == -1){
                perror("Cannot retrive queue");
                exit(EXIT_FAILURE);
        }
        return queue_id;
}

// Funtion to destroy a message queue using the system call msgctl
void remove_empty_queue(int queue_id){
        struct msqid_ds info;

        if(msgctl(queue_id, IPC_STAT, &info) == -1){
                perror("Cannot retrieve information about the message queue");
                exit(EXIT_FAILURE);
        }

        if(info.msg_qnum == 0){
                if(msgctl(queue_id, IPC_RMID, NULL) == -1){
                        perror("Cannot remove the message queue");
                        exit(EXIT_FAILURE);
                }
        }
        printf("Message queue removed\n");
}

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



int main(int argc,  char *argv[])
{
    char **rand_word;
    int len_of_strings = 10;
    int i=0;
    int num_strings = 50;
    int message_queue_id1;
    int message_queue_id2;
    struct message msg1;
    struct message_id msg2;

//allocates 50 pointers
    rand_word = (char **)malloc(num_strings*sizeof(char *)); 

 //created array of 50 random strings (of characters) of fixed length 10.
    for(i=0; i<num_strings; i++)
    {
       rand_word[i] = generate_random_string(len_of_strings);
//       printf("%s\n",rand_word[i]);
    }	    
    
    message_queue_id1 = get_message_queue(KEY1);
    message_queue_id2 = get_message_queue(KEY2);

    msg1.type=1;
    msg1.sender=getpid();

    printf("\n\n");
    for(i=0;i<10;i++)
    {
       // construct the message using 5 random strings along with their ID.

      snprintf(msg1.mtext,71,"\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n",
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
    
   //msgsnd is used to pass the the message to message queue
    if(msgsnd(message_queue_id1, &msg1, sizeof(msg1) - sizeof(long), 0) == -1 )
    { 
     perror("cannot send message with strings");
      exit(EXIT_FAILURE);
    }
   long type=0;

  // msgrcv is used to get the acknowldege id for the corresponding message from the message queue.
   if(msgrcv(message_queue_id2, &msg2, sizeof(msg2)-sizeof(long), type, 0) == -1)
   {
        perror("cannot receive message (acknowledge id)");
        exit(EXIT_FAILURE);
    }

    printf("\n\nAcknowledged id %d received for strings whose ID is from  %d to %d from process P2 with process id  %d\n",msg2.id,i*5,i*5+4,msg2.sender);
 
  i=i++;
    if(i==9)
	    break;

  }
	    
// remove_empty_queue(message_queue_id1);
 remove_empty_queue(message_queue_id2);


    return 0;
    
}
