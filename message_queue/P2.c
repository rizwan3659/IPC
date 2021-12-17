#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<stdlib.h>  
#include "message.h"
#include<unistd.h>

#define KEY1 0x01
#define KEY2 0x02

int get_message_queue(char id)
{
        key_t queue_key = ftok(".", id);
        printf("Created key %X\n", queue_key);

        // Get message queue
        int queue_id;
        if((queue_id = msgget(queue_key, IPC_CREAT | 0660)) == -1){
                perror("Cannot retrive queue");
                exit(EXIT_FAILURE);
        }
        return queue_id;
}

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
        printf("Message queue 2 removed\n");
}


  
int main()
{
  struct message msg1;
  struct message_id msg2;
  int message_queue_id1;
  int message_queue_id2;

   char **rand_word;
   int len_of_strings = 10;
    int i=0;
    int num_strings = 50;
    int a[5];
  message_queue_id1 = get_message_queue(KEY1);
  message_queue_id2 = get_message_queue(KEY2);
  
 rand_word = (char **)malloc(num_strings*sizeof(char *));

  for(i=0; i<num_strings; i++)
    {
       rand_word[i] = (char *)malloc(sizeof(len_of_strings+1));
    }


    long type=0;

    for( i=0;i<10;i++)
    {
      if(msgrcv(message_queue_id1, &msg1, sizeof(msg1) - sizeof(long), type, 0) == -1)
      {
        perror("msgrcv");
        exit(EXIT_FAILURE);
      }
      
      printf("\n\n");
      printf("Message %d containing strings with their IDs in the format (ID:string) received from process with PID (%d) %s\n\n",i+1,msg1.sender,msg1.mtext);
     sscanf(msg1.mtext,"\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n%d:%s\n",
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

    
     msg2.id = a[4];
     msg2.sender=getpid();
     msg2.type=1;
     if(msgsnd(message_queue_id2, &msg2, sizeof(msg2) - sizeof(long), 0)== -1)
      {
      perror("cannot send the acknowledge id");
      exit(EXIT_FAILURE);
       }

     sleep(1); 
   }
  remove_empty_queue(message_queue_id1);


    return 0;
}
