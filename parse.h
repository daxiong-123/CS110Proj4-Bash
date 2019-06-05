#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pwd.h>
#include<unistd.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>

#define max_arg_num 100

int parsecommand(char* cmdline, char **para_list, char *command);