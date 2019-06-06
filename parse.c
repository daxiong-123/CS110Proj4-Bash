#include "parse.h"
///////////////////////////////////////////////////////////////
struct listnode{
    int pid;
    char *command;
    int order;
    struct listnode *next;
}listnode;


///////////////////////////////////////////////////////////
int parsecommand(char* cmdline, char **command){

    if(cmdline[0] == '\n') 
        return 0; /* if input an empty line */

    int argnum = 0;
    cmdline = strtok(cmdline,"\n");

    command[0] = strtok(cmdline," "); /* first command */

    if(command[0] == NULL)
        return 0;

    argnum++;

    while(command[argnum-1] != NULL){
        command[argnum] = strtok(NULL," "); /* read parameter iterately */
        argnum++;
    }

    return argnum; /* return the num of parameter */
}


void ExecuteBuiltinCommand(char *command, char **para_list){
    struct passwd* user = getpwuid(getuid());
    if(strcmp(command,"cd")==0){
        if(para_list[0] == NULL || strcmp(para_list[0],"~") == 0){
            chdir(user->pw_dir);
        }
        else{
            chdir(para_list[0]);
        }  
    }

    else if(strcmp(command,"exit")){
        exit(0);
    }

//jobs 输出shell当前的一系列进程,必须提供子进程的命名和PID号
    else if(strcmp(command,"jobs")){
        /*do not know order*/
        // a lot of bugs
        listnode* tmp = head->next;
        listnode *p = (listnode*)malloc(sizeof(listnode));
        p->pid = pid;
        printf("%s", /*命令名*/);
        strcpy(p->command, /*命令名*/);

        head->next = p;
        p->next = tmp;

    }
    //需要一个num，还没仔细看是什么参数
    else if(strcmp(command,"kill")){
        
    }
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

int is_pipe(char **command, int argnum){
    for(int i=0; i<argnum; i++){
        if(command[i] != NULL && strcmp(command[i],"|") == 0){
            command[i] = NULL;
            return i+1;
        }
    }
    return 0;
}