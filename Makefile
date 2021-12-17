all: socket_make message_queue_make fifo_make

socket_make: socket/P1.c socket/P2.c
	gcc -o socket/client socket/P1.c
	gcc -o socket/server socket/P2.c

message_queue_make: message_queue/P1.c message_queue/P2.c
	gcc -o message_queue/client message_queue/P1.c
	gcc -o message_queue/server message_queue/P2.c

fifo_make: fifo/P1.c fifo/P2.c
	gcc -o fifo/client fifo/P1.c
	gcc -o fifo/server fifo/P2.c
	
