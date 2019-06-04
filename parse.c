#include "parse.h"

int parsecommand(char* cmdline, char **para_list, char *command){

    if(cmdline[0] == '\0') return -1; /* if input an empty line */

    char *cmd;
    int argnum = 0;

    cmd = strtok(cmdline," "); /* first command */
    command = cmd; /* return command str */

    while(cmd != NULL){
        cmd = strtok(NULL," ");
        para_list[argnum] = cmd; /* read parameter iterately */
        argnum++;
    }

    return argnum; /* return the num of parameter */
}

void ExecuteBuiltinCommand(char *command, char **para_list){
}

int isBuildincommand(char *cmd){
    if(strcmp(cmd,"cd") == 0)
        return 1; 
    else if(strcmp(cmd,"jobs") == 0)
        return 1;
    else if(strcmp(cmd,"exit") == 0)
        return 1;
    else if(strcmp(cmd,"kill") == 0)
        return 1;
    else
        return 0;
}