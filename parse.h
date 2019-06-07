#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<pwd.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>

#define max_arg_num 100


int parsecommand(char* cmdline, char **command);
int isBuildincommand(char *cmd);
void ExecuteBuiltinCommand(char *command, char **para_list);
int is_pipe(char **command, int argnum);

void insert(__pid_t pid, char *command);
void del(int num);
void create();

struct listnode{
    __pid_t pid;
    char command[100];
    struct listnode *next;
    struct listnode *prev;
}listnode;
