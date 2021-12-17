#include<sys/types.h>

/* shared message structure */

struct message {
        long type;
         pid_t sender;	
        char mtext[128];
};


struct message_id {
        long type; 
        pid_t sender;	
        int id;
};
