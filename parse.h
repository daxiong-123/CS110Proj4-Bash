#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pwd.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/stat.h>

#define max_arg_num 100


int parsecommand(char* cmdline, char **command);
int isBuildincommand(char *cmd);
void ExecuteBuiltinCommand(char *command, char **para_list);


