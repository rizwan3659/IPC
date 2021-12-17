SRC_P1=P1.c
EXE_P1=client
SRC_P2=P2.c
EXE_P2=server
CFLAGS=-g
cc=gcc

all: P1 P2

P1: ${SRC_P1}
	$(CC) ${CFLAGS} ${SRC_P1} -o ${EXE_P1}
P2: ${SRC_P2}
	$(CC) ${CFLAGS} ${SRC_P2} -o ${EXE_P2}
clean:
	rm -f ${EXE_P1} ${EXE_P2}
