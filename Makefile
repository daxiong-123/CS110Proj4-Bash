CC=gcc
CFLAGS= -g 


all: shell

parse.o: parse.c parse.h
	${CC} -c $< -o $@ ${CFLAGS} 

shell: shell.c parse.o
	${CC} $^ -o $@ ${CFLAGS}
	rm *.o

clean: 
	rm shell