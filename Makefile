CC=gcc
CFLAGS= -g -I /usr/lib/x86_64-linux-gnu -lreadline -ltermcap 


all: shell

parse.o: parse.c parse.h
	${CC} -c $< -o $@ ${CFLAGS} 

shell: shell.c parse.o
	${CC} $^ -o $@ ${CFLAGS}
	rm *.o

clean: 
	rm shell