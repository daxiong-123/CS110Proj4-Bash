CC=gcc
CFLAGS= -g -lreadline

all: shell

parse.o: parse.c parse.h
	${CC} ${CFLAGS} -c $< -o $@

shell: shell.c parse.o
	${CC} ${CFLAGS} shell.gnu