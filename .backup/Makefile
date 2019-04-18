CC = gcc
WARNINGS = -Wall -ansi -Wextra -pedantic

all: listener talker client server

listener: listener.c
	${CC} -D_GNU_SOURCE ${WARNINGS} -o exec_listener listener.c

talker: talker.c
	${CC} -D_GNU_SOURCE ${WARNINGS} -o exec_talker talker.c

client: client.c
	${CC} -D_GNU_SOURCE ${WARNINGS} -o exec_client client.c

server: server.c
	${CC} -D_GNU_SOURCE ${WARNINGS} -o exec_server server.c

clean : 
	rm -rf *.o *.d exec_*

mrproper : clean all